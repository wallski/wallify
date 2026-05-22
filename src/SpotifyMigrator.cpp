#include "SpotifyMigrator.h"
#include "LocalLibrary.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>
#include <QDirIterator>

SpotifyMigrator::SpotifyMigrator(LocalLibrary *library, QObject *parent)
    : QObject(parent), m_library(library), m_isWorking(false), m_progress(0), m_state(Idle)
{
    m_process = new QProcess(this);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &SpotifyMigrator::processOutput);
    connect(m_process, &QProcess::readyReadStandardError, this, &SpotifyMigrator::processOutput);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SpotifyMigrator::processFinished);

    m_appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(m_appDataDir);
    m_spotdlPath = m_appDataDir + "/spotdl.exe";

    QSettings settings("Wallski", "Wallify");
    m_musicDir = settings.value("libraryPath", QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify").toString();
    QDir().mkpath(m_musicDir);

    m_statusText = "Ready";
}

QString SpotifyMigrator::statusText() const { return m_statusText; }
bool SpotifyMigrator::isWorking() const { return m_isWorking; }
int SpotifyMigrator::progress() const { return m_progress; }
QString SpotifyMigrator::logs() const { return m_logs; }

void SpotifyMigrator::clearLogs()
{
    m_logs.clear();
    emit logsChanged();
}

void SpotifyMigrator::appendLog(const QString &message)
{
    m_logs += message + "
";
    emit logsChanged();
}

void SpotifyMigrator::setStatus(const QString &text)
{
    if (m_statusText != text) {
        m_statusText = text;
        emit statusTextChanged();
    }
}

void SpotifyMigrator::setWorking(bool working)
{
    if (m_isWorking != working) {
        m_isWorking = working;
        emit isWorkingChanged();
    }
}

void SpotifyMigrator::setProgress(int value)
{
    if (m_progress != value) {
        m_progress = value;
        emit progressChanged();
    }
}

void SpotifyMigrator::startMigration(const QString &url)
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

void SpotifyMigrator::prepareEnvironmentAndRun(const QString &url)
{
    if (!QFile::exists(m_spotdlPath)) {
        setStatus("Downloading spotDL core...");
        m_state = DownloadingSpotDL;
        m_process->start("curl.exe", {"-L", "https://github.com/spotDL/spotify-downloader/releases/download/v4.5.0/spotdl-4.5.0-win32.exe", "-o", m_spotdlPath});
        return;
    }

    QString spotdlFfmpeg = QDir::homePath() + "/.spotdl/ffmpeg.exe";
    QString spotdlFfmpegNoExt = QDir::homePath() + "/.spotdl/ffmpeg";

    if (!QFile::exists(m_appDataDir + "/ffmpeg.exe") && !QFile::exists(QDir::currentPath() + "/ffmpeg.exe") && !QFile::exists(spotdlFfmpeg) && !QFile::exists(spotdlFfmpegNoExt)) {
        setStatus("Setting up FFmpeg audio encoder... (this might take a while)");
        m_state = DownloadingFFmpeg;
        m_process->setWorkingDirectory(m_appDataDir);
        m_process->start(m_spotdlPath, {"--download-ffmpeg"});
        return;
    }

    QString statusType = "playlist";
    if (url.contains("/track/", Qt::CaseInsensitive)) {
        statusType = "track";
    } else if (url.contains("/album/", Qt::CaseInsensitive)) {
        statusType = "album";
    }

    setStatus("Migrating " + statusType + "...");
    m_state = Migrating;
    m_process->setWorkingDirectory(m_musicDir);
    m_process->start(m_spotdlPath, {url});
}

void SpotifyMigrator::processOutput()
{
    QByteArray output = m_process->readAllStandardOutput();
    QByteArray error = m_process->readAllStandardError();
    QString text = QString::fromUtf8(output).trimmed() + QString::fromUtf8(error).trimmed();

    if (text.isEmpty()) return;

    appendLog(text);
    emit logMessage(text);
    qDebug().noquote() << "[spotDL Output]:" << text;

    if (text.contains("(y/N)", Qt::CaseInsensitive)) {
        emit ffmpegOverwriteRequested();
    }

    if (m_state == Migrating) {
        if (text.contains("playlist '")) {
            int idx = text.indexOf("playlist '");
            int start = idx + 10;
            int end = text.indexOf("'", start);
            if (end != -1) {
                m_detectedPlaylistName = text.mid(start, end - start);
            }
        } else if (text.contains("album '")) {
            int idx = text.indexOf("album '");
            int start = idx + 7;
            int end = text.indexOf("'", start);
            if (end != -1) {
                m_detectedPlaylistName = text.mid(start, end - start);
            }
        }

        QRegularExpression progressRe("(\d+)%");
        QRegularExpressionMatch match = progressRe.match(text);
        if (match.hasMatch()) {
            setProgress(match.captured(1).toInt());
        }

        if (text.contains("Downloaded")) {
            QStringList lines = text.split('
');
            for (const QString &line : lines) {
                if (line.contains("Downloaded")) {
                    setStatus(line.trimmed());
                }
            }
        }
    }
}

void SpotifyMigrator::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::CrashExit) {
        setStatus("Process crashed.");
        setWorking(false);
        emit migrationFailed("Process crashed.");
        return;
    }

    if (m_state == DownloadingSpotDL) {
        if (exitCode == 0 && QFile::exists(m_spotdlPath)) {
            prepareEnvironmentAndRun(m_currentUrl);
        } else {
            setStatus("Failed to download spotDL.");
            setWorking(false);
            emit migrationFailed("Failed to download spotDL.");
        }
    } 
    else if (m_state == DownloadingFFmpeg) {
        prepareEnvironmentAndRun(m_currentUrl);
    } 
    else if (m_state == Migrating) {
        setProgress(100);
        setStatus("Migration Complete! Check Your Library.");
        setWorking(false);
        m_state = Idle;

        m_library->scan();

        QStringList filesAfter = scanFiles();
        QStringList newFiles;
        for (const QString &file : filesAfter) {
            if (!m_filesBefore.contains(file)) {
                newFiles.append(file);
            }
        }

        bool isPlaylistOrAlbum = m_currentUrl.contains("/playlist/", Qt::CaseInsensitive) || 
                                 m_currentUrl.contains("/album/", Qt::CaseInsensitive);

        if (isPlaylistOrAlbum && !newFiles.isEmpty()) {
            QString plName = m_detectedPlaylistName;
            if (plName.isEmpty()) {
                if (m_currentUrl.contains("/album/", Qt::CaseInsensitive)) {
                    plName = "Imported Album";
                } else {
                    plName = "Imported Playlist";
                }
            }
            m_library->createPlaylistFromMigration(plName, newFiles);
        }

        emit migrationCompleted();
    }
}

void SpotifyMigrator::answerFfmpegOverwrite(bool overwrite)
{
    if (m_process && m_process->state() == QProcess::Running) {
        if (overwrite) {
            m_process->write("Y
");
        } else {
            m_process->write("N
");
        }
    }
}

QStringList SpotifyMigrator::scanFiles()
{
    QStringList files;
    QDirIterator it(m_musicDir, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        files.append(it.next());
    }
    return files;
}
