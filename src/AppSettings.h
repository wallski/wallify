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

signals:
    void libraryPathChanged();
    void isFirstRunChanged();

private:
    QSettings m_settings;
};
