#ifndef LOCALLIBRARY_H
#define LOCALLIBRARY_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>
#include <QDir>
#include <QFile>
#include <QDateTime>

struct Track {
    QString filePath;
    QString title;
    QString artist;
    QString album;
    int duration = 0;
    bool hasCover = false;
    QDateTime dateAdded;
    qint64 coverUpdateTime = 0; // ms timestamp, used as QML image cache-buster
};

struct Playlist {
    QString id;
    QString name;
    QString coverPath;
    QStringList trackPaths;
    QDateTime dateCreated;
};

class LocalLibrary : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList tracks READ tracks NOTIFY libraryChanged)
    Q_PROPERTY(QString libraryPath READ libraryPath WRITE setLibraryPath NOTIFY libraryPathChanged)
    Q_PROPERTY(QVariantList playlists READ playlists NOTIFY playlistsChanged)
    Q_PROPERTY(int sortMode READ sortMode WRITE setSortMode NOTIFY sortModeChanged)

public:
    explicit LocalLibrary(QObject *parent = nullptr);

    QVariantList tracks() const;
    QVariantList playlists() const;
    QString libraryPath() const;
    void setLibraryPath(const QString &path);

    Q_INVOKABLE void scan();
    QByteArray getCoverData(const QString &filePath);

    Q_INVOKABLE void createPlaylist(const QString &name, const QString &coverPath = "");
    Q_INVOKABLE void deletePlaylist(const QString &id);
    Q_INVOKABLE void renamePlaylist(const QString &id, const QString &name);
    Q_INVOKABLE void updatePlaylistCover(const QString &id, const QString &coverPath);
    Q_INVOKABLE void addTrackToPlaylist(const QString &playlistId, const QString &filePath);
    Q_INVOKABLE void removeTrackFromPlaylist(const QString &playlistId, const QString &filePath);
    void createPlaylistFromMigration(const QString &playlistName, const QStringList &filePaths);

    Q_INVOKABLE void renameTrack(const QString &filePath, const QString &newTitle);
    Q_INVOKABLE void changeCover(const QString &filePath, const QString &imagePath);
    Q_INVOKABLE void deleteTrack(const QString &filePath);

    int sortMode() const;
    Q_INVOKABLE void setSortMode(int mode);

signals:
    void libraryChanged();
    void libraryPathChanged();
    void playlistsChanged();
    void sortModeChanged();

private:
    void scanDirectory();
    Track parseMp3Metadata(const QString &filePath);
    QString parseId3String(const char *data, int size);
    void writeId3Tag(const QString &filePath, const QString &title, const QString &artist, const QString &album, const QByteArray &newCoverData = QByteArray(), const QString &coverMimeType = "image/jpeg");

    void loadPlaylists();
    void savePlaylists();
    void sortTracks();

    QList<Track> m_tracks;
    QList<Playlist> m_playlists;
    QString m_libraryPath;

    QString m_defaultPlaylistName;
    QString m_defaultPlaylistCover;
    int m_sortMode = 0; // 0 = Custom (date added), 1 = Name, 2 = Album
};

#endif // LOCALLIBRARY_H
