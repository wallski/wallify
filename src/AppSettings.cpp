#include "AppSettings.h"
#include <QDir>
#include <QUrl>

AppSettings::AppSettings(QObject *parent)
    : QObject(parent), m_settings("Wallski", "Wallify")
{
    if (!m_settings.contains("libraryPath")) {
        QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify";
        QDir().mkpath(defaultPath);
        m_settings.setValue("libraryPath", defaultPath);
    }
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
