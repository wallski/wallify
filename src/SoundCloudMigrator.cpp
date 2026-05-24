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
    m_scDlPath = m_appDataDir + "/scdl.exe";

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
        setStatus("Downloading scdl...");
        m_process->start("curl.exe", QStringList() << "-L" << "https://github.com/flyingrub/scdl/releases/latest/download/scdl.exe" << "-o" << m_scDlPath);
        return;
    }

    setStatus("Downloading from SoundCloud...");
    m_process->setWorkingDirectory(m_musicDir);

    QStringList args;
    args << "-l" << url;
    args << "--path" << m_musicDir;
    args << "--onlymp3";

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
    qDebug().noquote() << "[scdl Output]:" << text;

    QRegularExpression progressRe("(\\d+)%");
    QRegularExpressionMatch match = progressRe.match(text);
    if (match.hasMatch()) {
        setProgress(match.captured(1).toInt());
    }

    if (text.contains("Downloading")) {
        setStatus(text.trimmed());
    }

    if (text.contains("playlist")) {
        QRegularExpression playlistRe("playlist\\s+(.+)");
        QRegularExpressionMatch plMatch = playlistRe.match(text);
        if (plMatch.hasMatch()) {
            m_detectedPlaylistName = plMatch.captured(1).trimmed();
        }
    }

    // Parse track titles/permalinks processed
    QStringList lines = text.split('\n');
    for (const QString& line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) continue;

        if (trimmed.contains("Downloading", Qt::CaseInsensitive)) {
            int idx = trimmed.indexOf("Downloading", 0, Qt::CaseInsensitive);
            if (idx != -1) {
                QString trackName = trimmed.mid(idx + 11).trimmed();
                if (!trackName.isEmpty()) {
                    m_playlistFiles.append(trackName);
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
            setStatus("Failed to download scdl.");
            setWorking(false);
            emit migrationFailed("Failed to download scdl.");
        }
        return;
    }

    setProgress(100);
    setStatus("Download Complete! Check Your Library.");
    setWorking(false);

    m_library->scan();

    QStringList playlistTracks;
    QStringList filesOnDisk = scanFiles();

    for (const QString& trackName : m_playlistFiles) {
        QString cleanTrack = trackName.toLower();
        cleanTrack.replace("-", " ").replace("_", " ");
        
        for (const QString& diskFile : filesOnDisk) {
            QString fileName = QFileInfo(diskFile).completeBaseName().toLower();
            fileName.replace("-", " ").replace("_", " ");
            
            if (fileName.contains(cleanTrack) || cleanTrack.contains(fileName)) {
                playlistTracks.append(diskFile);
                break;
            }
        }
    }

    // Fallback: If we couldn't parse individual tracks, use clean file differences
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