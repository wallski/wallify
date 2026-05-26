#include "SoundCloudMigrator.h"
#include "LocalLibrary.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QDirIterator>

SoundCloudMigrator::SoundCloudMigrator(LocalLibrary* library, QObject* parent)
    : QObject(parent), m_library(library), m_isWorking(false), m_progress(0)
{
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &SoundCloudMigrator::processOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &SoundCloudMigrator::processOutput);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
        this, &SoundCloudMigrator::processFinished);

    m_appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(m_appDataDir);
    m_scDlPath = m_appDataDir + "/yt-dlp.exe";

    QSettings settings("Wallski", "Wallify");
    m_musicDir = settings.value("libraryPath", QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify").toString();
    QDir().mkpath(m_musicDir);

    m_statusText = "Ready";
}

SoundCloudMigrator::~SoundCloudMigrator()
{
    if (m_process && m_process->state() != QProcess::NotRunning) {
        m_process->kill();
        m_process->waitForFinished(1000);
    }
}

QString SoundCloudMigrator::statusText() const { return m_statusText; }
bool SoundCloudMigrator::isWorking() const { return m_isWorking; }
int SoundCloudMigrator::progress() const { return m_progress; }
QString SoundCloudMigrator::logs() const { return m_logs; }

void SoundCloudMigrator::clearLogs()
{
    m_logs.clear();
    emit logsChanged();
}

void SoundCloudMigrator::appendLog(const QString& message)
{
    m_logs += message + "\n";
    emit logsChanged();
}

void SoundCloudMigrator::setStatus(const QString& text)
{
    if (m_statusText != text) {
        m_statusText = text;
        emit statusTextChanged();
    }
}

void SoundCloudMigrator::setWorking(bool working)
{
    if (m_isWorking != working) {
        m_isWorking = working;
        emit isWorkingChanged();
    }
}

void SoundCloudMigrator::setProgress(int value)
{
    if (m_progress != value) {
        m_progress = value;
        emit progressChanged();
    }
}

void SoundCloudMigrator::startMigration(const QString& url)
{
    if (m_isWorking) return;

    clearLogs();
    m_currentUrl = url;
    m_detectedPlaylistName.clear();
    m_playlistFiles.clear();

    m_filesBefore.clear();
    for (const QString& f : scanFiles()) {
        m_filesBefore.append(QDir::cleanPath(f));
    }

    setWorking(true);
    setProgress(0);

    prepareEnvironmentAndRun(url);
}

void SoundCloudMigrator::prepareEnvironmentAndRun(const QString& url)
{
    if (!QFile::exists(m_scDlPath)) {
        setStatus("Downloading yt-dlp...");
        m_process->start("curl.exe", QStringList() << "-L" << "https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe" << "-o" << m_scDlPath);
        return;
    }

    setStatus("Downloading from SoundCloud...");
    m_process->setWorkingDirectory(m_musicDir);

    QString ffmpegPath = m_appDataDir + "/ffmpeg.exe";
    if (!QFile::exists(ffmpegPath)) {
        QString spotdlFfmpeg = QDir::homePath() + "/.spotdl/ffmpeg.exe";
        if (QFile::exists(spotdlFfmpeg)) {
            QFile::copy(spotdlFfmpeg, ffmpegPath);
        }
    }

    QStringList args;
    args << "-x" << "--audio-format" << "mp3" << "--audio-quality" << "0";
    args << "--embed-thumbnail" << "--add-metadata";
    if (QFile::exists(ffmpegPath)) {
        args << "--ffmpeg-location" << ffmpegPath;
    }
    args << "--output" << "%(title)s - %(uploader)s.%(ext)s";
    args << url;

    m_process->start(m_scDlPath, args);
}

void SoundCloudMigrator::processOutput()
{
    QByteArray output = m_process->readAllStandardOutput();
    QByteArray error = m_process->readAllStandardError();
    QString text = QString::fromUtf8(output).trimmed() + QString::fromUtf8(error).trimmed();

    if (text.isEmpty()) return;

    appendLog(text);
    emit logMessage(text);
    qDebug().noquote() << "[yt-dlp SC Output]:" << text;

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

    QStringList lines = text.split('\n');
    for (const QString& line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) continue;

        if (trimmed.contains("has already been downloaded")) {
            QRegularExpression re("\\[download\\]\\s+(.+?)\\s+has already been downloaded");
            QRegularExpressionMatch match = re.match(trimmed);
            if (match.hasMatch()) {
                QString file = match.captured(1).trimmed();
                if (!file.endsWith(".mp3", Qt::CaseInsensitive)) file += ".mp3";
                QString fullPath = QDir(m_musicDir).absoluteFilePath(file);
                fullPath = QDir::cleanPath(fullPath);
                if (!m_playlistFiles.contains(fullPath)) {
                    m_playlistFiles.append(fullPath);
                    appendLog("Skipping (already downloaded): " + file);
                }
            }
        }
        else if (trimmed.contains("Destination:")) {
            QRegularExpression re("Destination:\\s+(.+?\\.mp3)");
            QRegularExpressionMatch match = re.match(trimmed);
            if (match.hasMatch()) {
                QString file = match.captured(1).trimmed();
                QString fullPath = QDir(m_musicDir).absoluteFilePath(file);
                fullPath = QDir::cleanPath(fullPath);
                if (!m_playlistFiles.contains(fullPath)) {
                    m_playlistFiles.append(fullPath);
                }
            }
        }
    }
}

void SoundCloudMigrator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        setStatus("Process crashed.");
        setWorking(false);
        emit migrationFailed("Process crashed.");
        return;
    }

    if (!QFile::exists(m_scDlPath)) {
        if (exitCode == 0 && QFile::exists(m_scDlPath)) {
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

    QStringList playlistTracks;
    QStringList filesOnDisk = scanFiles();

    for (const QString& file : m_playlistFiles) {
        QString cleanPath = QDir::cleanPath(file);
        for (const QString& diskFile : filesOnDisk) {
            if (QDir::cleanPath(diskFile).toLower() == cleanPath.toLower()) {
                playlistTracks.append(diskFile);
                break;
            }
        }
    }

    if (playlistTracks.isEmpty()) {
        for (const QString& file : filesOnDisk) {
            QString cleanFile = QDir::cleanPath(file);
            if (!m_filesBefore.contains(cleanFile)) {
                playlistTracks.append(cleanFile);
            }
        }
    }

    if (!playlistTracks.isEmpty() && m_currentUrl.contains("sets", Qt::CaseInsensitive)) {
        QString plName = m_detectedPlaylistName;
        if (plName.isEmpty()) plName = "SoundCloud Playlist";
        m_library->createPlaylistFromMigration(plName, playlistTracks);
    }

    emit migrationCompleted();
}

QStringList SoundCloudMigrator::scanFiles()
{
    QStringList files;
    QDirIterator it(m_musicDir, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        files.append(it.next());
    }
    return files;
}