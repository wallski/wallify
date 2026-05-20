#include "AppSettings.h"
#include <QDir>
#include <QUrl>
#include <QStandardPaths>
#include <QFile>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

AppSettings::AppSettings(QObject *parent)
    : QObject(parent), m_settings("Wallski", "Wallify")
{
    if (!m_settings.contains("libraryPath")) {
        QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify";
        QDir().mkpath(defaultPath);
        m_settings.setValue("libraryPath", defaultPath);
    }
    
    // Ensure music directory exists
    QDir().mkpath(libraryPath());
}

QString AppSettings::libraryPath() const
{
    return m_settings.value("libraryPath").toString();
}

void AppSettings::setLibraryPath(const QString &path)
{
    QString localPath = path;
    if (path.startsWith("file:///")) {
        localPath = QUrl(path).toLocalFile();
    }
    else if (path.startsWith("file://")) {
        localPath = QUrl(path).toLocalFile();
    }
    
    if (libraryPath() != localPath) {
        m_settings.setValue("libraryPath", localPath);
        emit libraryPathChanged();
    }
}

bool AppSettings::isFirstRun() const
{
    return m_settings.value("isFirstRun", true).toBool();
}

void AppSettings::setIsFirstRun(bool firstRun)
{
    if (isFirstRun() != firstRun) {
        m_settings.setValue("isFirstRun", firstRun);
        emit isFirstRunChanged();
    }
}

bool AppSettings::showDebugWindow() const
{
    return m_settings.value("showDebugWindow", false).toBool();
}

void AppSettings::setShowDebugWindow(bool show)
{
    if (showDebugWindow() != show) {
        m_settings.setValue("showDebugWindow", show);
        emit showDebugWindowChanged();
    }
}

void AppSettings::factoryReset()
{
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    
    // Delete spotdl.exe, ffmpeg.exe and playlists.json
    QFile::remove(appDataDir + "/spotdl.exe");
    QFile::remove(appDataDir + "/ffmpeg.exe");
    QFile::remove(appDataDir + "/playlists.json");
    
    // Delete ffmpeg folder
    QDir(QDir::homePath() + "/.spotdl").removeRecursively();
    
    setIsFirstRun(true);
    emit factoryResetRequested();
}
