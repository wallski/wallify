#pragma once

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStandardPaths>
#include <QDir>

class LocalLibrary;

class SpotifyMigrator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString logs READ logs NOTIFY logsChanged)

public:
    explicit SpotifyMigrator(LocalLibrary *library, QObject *parent = nullptr);
    ~SpotifyMigrator();

    QString statusText() const;
    bool isWorking() const;
    int progress() const;
    QString logs() const;

    Q_INVOKABLE void startMigration(const QString &url);
    Q_INVOKABLE void answerFfmpegOverwrite(bool overwrite);
    Q_INVOKABLE void clearLogs();

signals:
    void statusTextChanged();
    void isWorkingChanged();
    void progressChanged();
    void migrationCompleted();
    void migrationFailed(const QString &error);
    void logMessage(const QString &message);
    void ffmpegOverwriteRequested();
    void logsChanged();

private slots:
    void processOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void setStatus(const QString &text);
    void setWorking(bool working);
    void setProgress(int value);
    void appendLog(const QString &message);

    void prepareEnvironmentAndRun(const QString &url);
    QStringList scanFiles();

    LocalLibrary *m_library;
    QProcess *m_process;
    QString m_statusText;
    bool m_isWorking;
    int m_progress;
    QString m_logs;
    QString m_appDataDir;
    QString m_spotdlPath;
    QString m_musicDir;
    
    // Internal state tracking
    enum State { Idle, DownloadingSpotDL, DownloadingFFmpeg, Migrating };
    State m_state;
    QString m_currentUrl;
    QStringList m_filesBefore;
    QString m_detectedPlaylistName;
    QStringList m_playlistFiles;
};
