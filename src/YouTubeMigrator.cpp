#include "YouTubeMigrator.h"
#include "LocalLibrary.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QDirIterator>

YouTubeMigrator::YouTubeMigrator(LocalLibrary* library, QObject* parent)
    : QObject(parent), m_library(library), m_isWorking(false), m_progress(0)
{
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &YouTubeMigrator::processOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &YouTubeMigrator::processOutput);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &YouTubeMigrator::processFinished);

    m_appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(m_appDataDir);
    m_ytDlpPath = m_appDataDir + "/yt-dlp.exe";

    QSettings settings("Wallski", "Wallify");
    m_musicDir = settings.value("libraryPath", QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify").toString();
    QDir().mkpath(m_musicDir);

    m_statusText = "Ready";
}

QString YouTubeMigrator::statusText() const { return m_statusText; }
bool YouTubeMigrator::isWorking() const { return m_isWorking; }
int YouTubeMigrator::progress() const { return m_progress; }
QString YouTubeMigrator::logs() const { return m_logs; }

void YouTubeMigrator::clearLogs()
{
    m_logs.clear();
    emit logsChanged();
}

void YouTubeMigrator::appendLog(const QString& message)
{
    m_logs += message + "\n";
    emit logsChanged();
}

void YouTubeMigrator::setStatus(const QString& text)
{
    if (m_statusText != text) {
        m_statusText = text;
        emit statusTextChanged();
    }
}

void YouTubeMigrator::setWorking(bool working)
{
    if (m_isWorking != working) {
        m_isWorking = working;
        emit isWorkingChanged();
    }
}

void YouTubeMigrator::setProgress(int value)
{
    if (m_progress != value) {
        m_progress = value;
        emit progressChanged();
    }
}

void YouTubeMigrator::startMigration(const QString& url)
{
    if (m_isWorking) return;

    clearLogs();
    m_currentUrl = url;
    m_detectedPlaylistName.clear();
    m_filesBefore = scanFiles();

    setWorking(true);
    setProgress(0);

    prepareEnvironmentAndRun(url);
}

void YouTubeMigrator::prepareEnvironmentAndRun(const QString& url)
{
    if (!QFile::exists(m_ytDlpPath)) {
        setStatus("Downloading yt-dlp...");
        m_process->start("curl.exe", QStringList() << "-L" << "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe" << "-o" << m_ytDlpPath);
        return;
    }

    setStatus("Downloading from YouTube...");
    m_process->setWorkingDirectory(m_musicDir);

    QStringList args;
    args << "-x" << "--audio-format" << "mp3" << "--audio-quality" << "0";
    args << "--embed-thumbnail" << "--add-metadata";
    args << "--output" << "%(title)s - %(uploader)s.%(ext)s";
    args << url;

    m_process->start(m_ytDlpPath, args);
}

void YouTubeMigrator::processOutput()
{
    QByteArray output = m_process->readAllStandardOutput();
    QByteArray error = m_process->readAllStandardError();
    QString text = QString::fromUtf8(output).trimmed() + QString::fromUtf8(error).trimmed();

    if (text.isEmpty()) return;

    appendLog(text);
    emit logMessage(text);
    qDebug().noquote() << "[yt-dlp Output]:" << text;

    QRegularExpression progressRe("\\[download\\]\\s+(\\d+\\.\\d+)%");
    QRegularExpressionMatch match = progressRe.match(text);
    if (match.hasMatch()) {
        setProgress(static_cast<int>(match.captured(1).toDouble()));
    }

    if (text.contains("Destination:")) {
        QStringList parts = text.split("Destination:");
        if (parts.size() > 1) {
            setStatus("Downloading: " + parts.last().trimmed());
        }
    }

    if (text.contains("playlist")) {
        QRegularExpression playlistRe("playlist\\s+(.+)");
        QRegularExpressionMatch plMatch = playlistRe.match(text);
        if (plMatch.hasMatch()) {
            m_detectedPlaylistName = plMatch.captured(1).trimmed();
        }
    }
}

void YouTubeMigrator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        setStatus("Process crashed.");
        setWorking(false);
        emit migrationFailed("Process crashed.");
        return;
    }

    if (!QFile::exists(m_ytDlpPath)) {
        if (exitCode == 0 && QFile::exists(m_ytDlpPath)) {
            prepareEnvironmentAndRun(m_currentUrl);
        }
        else {
            setStatus("Failed to download yt-dlp.");
            setWorking(false);
            emit migrationFailed("Failed to download yt-dlp.");
        }
        return;
    }

    setProgress(100);
    setStatus("Download Complete! Check Your Library.");
    setWorking(false);

    m_library->scan();

    QStringList filesAfter = scanFiles();
    QStringList newFiles;
    for (const QString& file : filesAfter) {
        if (!m_filesBefore.contains(file)) {
            newFiles.append(file);
        }
    }

    if (!newFiles.isEmpty() && m_currentUrl.contains("playlist", Qt::CaseInsensitive)) {
        QString plName = m_detectedPlaylistName;
        if (plName.isEmpty()) plName = "YouTube Playlist";
        m_library->createPlaylistFromMigration(plName, newFiles);
    }

    emit migrationCompleted();
}

QStringList YouTubeMigrator::scanFiles()
{
    QStringList files;
    QDirIterator it(m_musicDir, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        files.append(it.next());
    }
    return files;
}