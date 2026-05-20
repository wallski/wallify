#pragma once

#include <QObject>
#include <QSettings>
#include <QString>
#include <QStandardPaths>

class AppSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString libraryPath READ libraryPath WRITE setLibraryPath NOTIFY libraryPathChanged)
    Q_PROPERTY(bool isFirstRun READ isFirstRun WRITE setIsFirstRun NOTIFY isFirstRunChanged)

public:
    explicit AppSettings(QObject *parent = nullptr);

    QString libraryPath() const;
    void setLibraryPath(const QString &path);
    
    bool isFirstRun() const;
    Q_INVOKABLE void setIsFirstRun(bool firstRun);
    
    Q_PROPERTY(bool showDebugWindow READ showDebugWindow WRITE setShowDebugWindow NOTIFY showDebugWindowChanged)
    bool showDebugWindow() const;
    Q_INVOKABLE void setShowDebugWindow(bool show);
    
    Q_INVOKABLE void factoryReset();

signals:
    void libraryPathChanged();
    void isFirstRunChanged();
    void showDebugWindowChanged();
    void factoryResetRequested();

private:
    QSettings m_settings;
};
