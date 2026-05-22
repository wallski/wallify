#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>

#ifdef _WIN32
#include <windows.h>
#endif

class DiscordRPC : public QObject
{
    Q_OBJECT

public:
    explicit DiscordRPC(QObject *parent = nullptr);
    ~DiscordRPC();

    Q_INVOKABLE void initialize();
    Q_INVOKABLE void shutdown();
    Q_INVOKABLE void updatePresence(const QString &state, const QString &details, 
                                     const QString &largeImageKey, const QString &largeImageText,
                                     qint64 startTimestamp, qint64 endTimestamp);
    Q_INVOKABLE void clearPresence();

private:
    bool m_initialized;
    QTimer *m_updateTimer;

    void setupCallbacks();
    static void handleReady(const void *userData);
    static void handleDisconnected(int errcode, const char *message, void *userData);
    static void handleError(int errcode, const char *message, void *userData);
};
