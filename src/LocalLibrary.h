#ifndef LOCALLIBRARY_H
#define LOCALLIBRARY_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QDir>
#include <QFile>

struct Track {
    QString filePath;
    QString title;
    QString artist;
    QString album;
    int duration = 0;
    bool hasCover = false;
};

struct Playlist {
    QString id;
    QString name;
    QString coverPath;
    QStringList trackPaths;
};

class LocalLibrary : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList tracks READ tracks NOTIFY libraryChanged)
    Q_PROPERTY(QString libraryPath READ libraryPath WRITE setLibraryPath NOTIFY libraryPathChanged)
    Q_PROPERTY(QVariantList playlists READ playlists NOTIFY playlistsChanged)

public:
    explicit LocalLibrary(QObject *parent = nullptr);

    QVariantList tracks() const;
    QVariantList playlists() const;
    QString libraryPath() const;
    void setLibraryPath(const QString &path);

    Q_INVOKABLE void scan();
    QByteArray getCoverData(const QString &filePath);

    // Playlist management
    Q_INVOKABLE void createPlaylist(const QString &name, const QString &coverPath = "");
    Q_INVOKABLE void deletePlaylist(const QString &id);
    Q_INVOKABLE void renamePlaylist(const QString &id, const QString &name);
    Q_INVOKABLE void updatePlaylistCover(const QString &id, const QString &coverPath);
    Q_INVOKABLE void addTrackToPlaylist(const QString &playlistId, const QString &filePath);
    Q_INVOKABLE void removeTrackFromPlaylist(const QString &playlistId, const QString &filePath);
    void createPlaylistFromMigration(const QString &playlistName, const QStringList &filePaths);

signals:
    void libraryChanged();
    void libraryPathChanged();
    void playlistsChanged();

private:
    void scanDirectory();
    Track parseMp3Metadata(const QString &filePath);
    QString parseId3String(const char *data, int size);
    
    void loadPlaylists();
    void savePlaylists();

    QList<Track> m_tracks;
    QList<Playlist> m_playlists;
    QString m_libraryPath;
    
    QString m_defaultPlaylistName;
    QString m_defaultPlaylistCover;
};

#endif // LOCALLIBRARY_H
