#pragma once

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStandardPaths>
#include <QDir>

class SpotifyMigrator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged)

public:
    explicit SpotifyMigrator(QObject *parent = nullptr);

    QString statusText() const;
    bool isWorking() const;
    int progress() const;

    Q_INVOKABLE void startMigration(const QString &url);
    Q_INVOKABLE void answerFfmpegOverwrite(bool overwrite);

signals:
    void statusTextChanged();
    void isWorkingChanged();
    void progressChanged();
    void migrationCompleted();
    void migrationFailed(const QString &error);
    void logMessage(const QString &message);
    void ffmpegOverwriteRequested();

private slots:
    void processOutput();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    void setStatus(const QString &text);
    void setWorking(bool working);
    void setProgress(int value);

    void prepareEnvironmentAndRun(const QString &url);

    QProcess *m_process;
    QString m_statusText;
    bool m_isWorking;
    int m_progress;
    QString m_appDataDir;
    QString m_spotdlPath;
    QString m_musicDir;
    
    // Internal state tracking
    enum State { Idle, DownloadingSpotDL, DownloadingFFmpeg, Migrating };
    State m_state;
    QString m_currentUrl;
};
