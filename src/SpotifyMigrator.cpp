#include "SpotifyMigrator.h"
#include <QDebug>
#include <QRegularExpression>
#include <QSettings>

SpotifyMigrator::SpotifyMigrator(QObject *parent)
    : QObject(parent), m_isWorking(false), m_progress(0), m_state(Idle)
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
    
    m_currentUrl = url;
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

    // SpotDL downloads ffmpeg to ~/.spotdl/ffmpeg.exe or ~/.spotdl/ffmpeg
    QString spotdlFfmpeg = QDir::homePath() + "/.spotdl/ffmpeg.exe";
    QString spotdlFfmpegNoExt = QDir::homePath() + "/.spotdl/ffmpeg";
    
    if (!QFile::exists(m_appDataDir + "/ffmpeg.exe") && !QFile::exists(QDir::currentPath() + "/ffmpeg.exe") && !QFile::exists(spotdlFfmpeg) && !QFile::exists(spotdlFfmpegNoExt)) {
        setStatus("Setting up FFmpeg audio encoder...");
        m_state = DownloadingFFmpeg;
        m_process->setWorkingDirectory(m_appDataDir);
        m_process->start(m_spotdlPath, {"--download-ffmpeg"});
        return;
    }

    setStatus("Migrating playlist...");
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
    
    // Print everything to the internal log list
    emit logMessage(text);
    qDebug().noquote() << "[spotDL Output]:" << text;
    
    // If it ever asks for confirmation, emit a signal for QML instead of blindly answering No
    if (text.contains("(y/N)", Qt::CaseInsensitive)) {
        emit ffmpegOverwriteRequested();
    }
    
    if (m_state == Migrating) {
        // spotdl output looks like: 
        // 100%|████████████| 1/1 [00:00<00:00, 298.54it/s]
        
        QRegularExpression progressRe("(\\d+)%");
        QRegularExpressionMatch match = progressRe.match(text);
        if (match.hasMatch()) {
            setProgress(match.captured(1).toInt());
        }
        
        if (text.contains("Downloaded")) {
            // Keep status update clean
            QStringList lines = text.split('\n');
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
        // Even if exit code isn't 0, if ffmpeg exists we can try to proceed
        prepareEnvironmentAndRun(m_currentUrl);
    } 
    else if (m_state == Migrating) {
        setProgress(100);
        setStatus("Migration Complete! Check Your Library.");
        setWorking(false);
        m_state = Idle;
        emit migrationCompleted();
    }
}

void SpotifyMigrator::answerFfmpegOverwrite(bool overwrite)
{
    if (m_process && m_process->state() == QProcess::Running) {
        if (overwrite) {
            m_process->write("Y\n");
        } else {
            m_process->write("N\n");
        }
    }
}
