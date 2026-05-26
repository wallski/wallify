#include "LocalLibrary.h"
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <cstring>
#include <algorithm>

LocalLibrary::LocalLibrary(QObject *parent)
    : QObject(parent)
{
    QSettings settings("Wallski", "Wallify");
    m_libraryPath = settings.value("libraryPath", QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify").toString();
    m_sortMode = settings.value("sortMode", 0).toInt();
    QDir().mkpath(m_libraryPath);
    loadPlaylists();
    scanDirectory();
}

QVariantList LocalLibrary::tracks() const
{
    QVariantList list;
    for (const Track &t : m_tracks) {
        QVariantMap map;
        map["filePath"] = t.filePath;
        map["title"] = t.title;
        map["artist"] = t.artist;
        map["album"] = t.album;
        map["duration"] = t.duration;
        map["hasCover"] = t.hasCover;
        map["dateAdded"] = t.dateAdded.toString(Qt::ISODate);
        list.append(map);
    }
    return list;
}

QVariantList LocalLibrary::playlists() const
{
    QVariantList list;

    QVariantMap defMap;
    defMap["id"] = "default";
    defMap["name"] = m_defaultPlaylistName;
    defMap["coverPath"] = m_defaultPlaylistCover;
    defMap["isDefault"] = true;

    QVariantList defTracks;
    for (const Track &t : m_tracks) {
        QVariantMap tMap;
        tMap["filePath"] = t.filePath;
        tMap["title"] = t.title;
        tMap["artist"] = t.artist;
        tMap["album"] = t.album;
        tMap["duration"] = t.duration;
        tMap["hasCover"] = t.hasCover;
        tMap["dateAdded"] = t.dateAdded.toString(Qt::ISODate);
        defTracks.append(tMap);
    }
    defMap["tracks"] = defTracks;
    list.append(defMap);

    for (const Playlist &p : m_playlists) {
        QVariantMap pMap;
        pMap["id"] = p.id;
        pMap["name"] = p.name;
        pMap["coverPath"] = p.coverPath;
        pMap["isDefault"] = false;
        pMap["dateCreated"] = p.dateCreated.toString(Qt::ISODate);

        QVariantList pTracks;
        for (const QString &path : p.trackPaths) {
            bool found = false;
            for (const Track &t : m_tracks) {
                if (t.filePath == path) {
                    QVariantMap tMap;
                    tMap["filePath"] = t.filePath;
                    tMap["title"] = t.title;
                    tMap["artist"] = t.artist;
                    tMap["album"] = t.album;
                    tMap["duration"] = t.duration;
                    tMap["hasCover"] = t.hasCover;
                    tMap["dateAdded"] = t.dateAdded.toString(Qt::ISODate);
                    pTracks.append(tMap);
                    found = true;
                    break;
                }
            }
        }

        // Sort the custom playlist tracks if a sort mode is active
        if (m_sortMode == 1) { // Name
            std::sort(pTracks.begin(), pTracks.end(), [](const QVariant &a, const QVariant &b) {
                return a.toMap()["title"].toString().toLower() < b.toMap()["title"].toString().toLower();
            });
        } else if (m_sortMode == 2) { // Album
            std::sort(pTracks.begin(), pTracks.end(), [](const QVariant &a, const QVariant &b) {
                QString albumA = a.toMap()["album"].toString().toLower();
                QString albumB = b.toMap()["album"].toString().toLower();
                if (albumA != albumB)
                    return albumA < albumB;
                return a.toMap()["title"].toString().toLower() < b.toMap()["title"].toString().toLower();
            });
        }

        pMap["tracks"] = pTracks;
        list.append(pMap);
    }

    return list;
}

QString LocalLibrary::libraryPath() const
{
    return m_libraryPath;
}

void LocalLibrary::setLibraryPath(const QString &path)
{
    if (m_libraryPath != path) {
        m_libraryPath = path;
        QDir().mkpath(m_libraryPath);
        emit libraryPathChanged();
        scan();
    }
}

void LocalLibrary::scan()
{
    scanDirectory();
}

void LocalLibrary::scanDirectory()
{
    m_tracks.clear();
    if (m_libraryPath.isEmpty() || !QDir(m_libraryPath).exists()) {
        emit libraryChanged();
        emit playlistsChanged();
        return;
    }

    QDirIterator it(m_libraryPath, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        Track track = parseMp3Metadata(filePath);
        QFileInfo info(filePath);
        track.dateAdded = info.birthTime().isValid() ? info.birthTime() : info.metadataChangeTime();
        m_tracks.append(track);
    }

    sortTracks();
    emit libraryChanged();
    emit playlistsChanged();
}

void LocalLibrary::sortTracks()
{
    switch (m_sortMode) {
    case 1: // Name
        std::sort(m_tracks.begin(), m_tracks.end(), [](const Track &a, const Track &b) {
            return a.title.toLower() < b.title.toLower();
        });
        break;
    case 2: // Album
        std::sort(m_tracks.begin(), m_tracks.end(), [](const Track &a, const Track &b) {
            if (a.album.toLower() != b.album.toLower())
                return a.album.toLower() < b.album.toLower();
            return a.title.toLower() < b.title.toLower();
        });
        break;
    default: // Custom (date added)
        std::sort(m_tracks.begin(), m_tracks.end(), [](const Track &a, const Track &b) {
            return a.dateAdded > b.dateAdded;
        });
        break;
    }
}

int LocalLibrary::sortMode() const
{
    return m_sortMode;
}

void LocalLibrary::setSortMode(int mode)
{
    if (m_sortMode != mode) {
        m_sortMode = mode;
        QSettings settings("Wallski", "Wallify");
        settings.setValue("sortMode", m_sortMode);
        sortTracks();
        emit sortModeChanged();
        emit libraryChanged();
        emit playlistsChanged();
    }
}

static int parseMp3Duration(const QString &filePath, int tagSize)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return 0;

    if (!file.seek(tagSize)) {
        file.close();
        return 0;
    }

    QByteArray buffer = file.read(4096);
    file.close();

    int bufferOffset = 0;
    int bitrate = 0;

    while (bufferOffset + 4 < buffer.size()) {
        uchar b1 = buffer[bufferOffset];
        uchar b2 = buffer[bufferOffset + 1];
        if (b1 == 0xFF && (b2 & 0xE0) == 0xE0) {
            int version = (b2 & 0x18) >> 3;
            int bitrateIdx = (buffer[bufferOffset + 2] & 0xF0) >> 4;
            int srIdx = (buffer[bufferOffset + 2] & 0x0C) >> 2;

            if (bitrateIdx == 0 || bitrateIdx == 15 || srIdx == 3) {
                bufferOffset++;
                continue;
            }

            int bitrateTableM1L3[16] = { 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 };
            int bitrateTableM2L3[16] = { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 };

            if (version == 3) {
                bitrate = bitrateTableM1L3[bitrateIdx];
            } else {
                bitrate = bitrateTableM2L3[bitrateIdx];
            }
            break;
        }
        bufferOffset++;
    }

    if (bitrate > 0) {
        QFileInfo info(filePath);
        qint64 audioSize = info.size() - tagSize;
        if (audioSize > 0) {
            return static_cast<int>((audioSize * 8) / bitrate);
        }
    }
    return 0;
}

Track LocalLibrary::parseMp3Metadata(const QString &filePath)
{
    Track track;
    track.filePath = filePath;

    QFileInfo fileInfo(filePath);
    QString baseName = fileInfo.completeBaseName();
    int hyphenIdx = baseName.indexOf(" - ");
    if (hyphenIdx != -1) {
        track.artist = baseName.left(hyphenIdx).trimmed();
        track.title = baseName.mid(hyphenIdx + 3).trimmed();
    } else {
        track.artist = "Unknown Artist";
        track.title = baseName;
    }
    track.album = "Unknown Album";
    track.hasCover = false;
    track.duration = 0;

    QFile file(filePath);
    qint64 totalId3Size = 0;
    if (file.open(QIODevice::ReadOnly)) {
        char header[10];
        if (file.read(header, 10) == 10 && strncmp(header, "ID3", 3) == 0) {
            int majorVersion = header[3];

            quint32 tagSize = ((header[6] & 0x7F) << 21) |
                              ((header[7] & 0x7F) << 14) |
                              ((header[8] & 0x7F) << 7)  |
                              (header[9] & 0x7F);

            totalId3Size = 10 + tagSize;
            QByteArray tagData = file.read(tagSize);

            if ((header[5] & 0x80) != 0) {
                QByteArray decoded;
                decoded.reserve(tagData.size());
                for (int i = 0; i < tagData.size(); ++i) {
                    decoded.append(tagData[i]);
                    if (i + 1 < tagData.size() && static_cast<uchar>(tagData[i]) == 0xFF && static_cast<uchar>(tagData[i+1]) == 0x00) {
                        i++;
                    }
                }
                tagData = decoded;
            }

            int tagOffset = 0;
            if ((header[5] & 0x40) != 0 && tagData.size() >= 4) {
                quint32 extHeaderSize = 0;
                if (majorVersion == 4) {
                    extHeaderSize = ((tagData[0] & 0x7F) << 21) |
                                    ((tagData[1] & 0x7F) << 14) |
                                    ((tagData[2] & 0x7F) << 7)  |
                                    (tagData[3] & 0x7F);
                    tagOffset += extHeaderSize;
                } else {
                    extHeaderSize = (static_cast<uchar>(tagData[0]) << 24) |
                                    (static_cast<uchar>(tagData[1]) << 16) |
                                    (static_cast<uchar>(tagData[2]) << 8)  |
                                    static_cast<uchar>(tagData[3]);
                    tagOffset += 4 + extHeaderSize;
                }
            }

            int offset = tagOffset;
            while (offset + (majorVersion == 2 ? 6 : 10) < tagData.size()) {
                const char *frameHeader = tagData.constData() + offset;
                QByteArray frameId;
                quint32 frameSize = 0;
                int headerSize = 0;

                if (majorVersion == 2) {
                    frameId = QByteArray(frameHeader, 3);
                    frameSize = (static_cast<uchar>(frameHeader[3]) << 16) |
                                (static_cast<uchar>(frameHeader[4]) << 8)  |
                                static_cast<uchar>(frameHeader[5]);
                    headerSize = 6;
                } else {
                    frameId = QByteArray(frameHeader, 4);
                    if (majorVersion == 4) {
                        frameSize = ((frameHeader[4] & 0x7F) << 21) |
                                    ((frameHeader[5] & 0x7F) << 14) |
                                    ((frameHeader[6] & 0x7F) << 7)  |
                                    (frameHeader[7] & 0x7F);
                    } else {
                        frameSize = (static_cast<uchar>(frameHeader[4]) << 24) |
                                    (static_cast<uchar>(frameHeader[5]) << 16) |
                                    (static_cast<uchar>(frameHeader[6]) << 8)  |
                                    static_cast<uchar>(frameHeader[7]);
                    }
                    headerSize = 10;
                }

                if (frameId.trimmed().isEmpty()) break;

                offset += headerSize;
                if (offset + frameSize > static_cast<quint32>(tagData.size())) break;

                const char *frameData = tagData.constData() + offset;

                if (frameId == "TIT2" || frameId == "TT2") {
                    QString parsedTitle = parseId3String(frameData, frameSize);
                    if (!parsedTitle.isEmpty()) track.title = parsedTitle;
                } else if (frameId == "TPE1" || frameId == "TP1") {
                    QString parsedArtist = parseId3String(frameData, frameSize);
                    if (!parsedArtist.isEmpty()) track.artist = parsedArtist;
                } else if (frameId == "TALB" || frameId == "TAL") {
                    QString parsedAlbum = parseId3String(frameData, frameSize);
                    if (!parsedAlbum.isEmpty()) track.album = parsedAlbum;
                } else if (frameId == "APIC" || frameId == "PIC") {
                    track.hasCover = true;
                } else if (frameId == "TLEN" || frameId == "TLE") {
                    QString lenStr = parseId3String(frameData, frameSize);
                    bool ok;
                    int ms = lenStr.toInt(&ok);
                    if (ok) track.duration = ms;
                }

                offset += frameSize;
            }
        }
        file.close();
    }

    if (track.duration <= 0) {
        track.duration = parseMp3Duration(filePath, totalId3Size);
    }

    return track;
}

QString LocalLibrary::parseId3String(const char *data, int size)
{
    if (size <= 1) return QString();
    uchar encoding = data[0];
    const char *textData = data + 1;
    int textSize = size - 1;

    if (encoding == 1 || encoding == 2) {
        // Strip 16-bit null terminators (2 bytes at a time) to avoid dropping valid half-characters
        while (textSize >= 2 && textData[textSize - 1] == 0 && textData[textSize - 2] == 0) {
            textSize -= 2;
        }
    } else {
        // Strip 8-bit null terminators
        while (textSize > 0 && textData[textSize - 1] == 0) {
            textSize--;
        }
    }

    if (encoding == 0) {
        return QString::fromLatin1(textData, textSize).trimmed();
    } else if (encoding == 1) {
        if (textSize >= 2) {
            const uchar *bom = reinterpret_cast<const uchar*>(textData);
            if ((bom[0] == 0xFF && bom[1] == 0xFE) || (bom[0] == 0xFE && bom[1] == 0xFF)) {
                textData += 2;
                textSize -= 2;
            }
        }
        return QString::fromUtf16(reinterpret_cast<const char16_t*>(textData), textSize / 2).trimmed();
    } else if (encoding == 2) {
        return QString::fromUtf16(reinterpret_cast<const char16_t*>(textData), textSize / 2).trimmed();
    } else if (encoding == 3) {
        return QString::fromUtf8(textData, textSize).trimmed();
    }
    return QString::fromLatin1(textData, textSize).trimmed();
}

void LocalLibrary::writeId3Tag(const QString &filePath, const QString &title, const QString &artist, const QString &album, const QByteArray &newCoverData)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray fileData = file.readAll();
    file.close();

    QByteArray newTag;
    newTag.append("ID3");
    newTag.append(char(3)); // use ID3v2.3
    newTag.append(char(0));
    newTag.append(char(0));

    QByteArray frames;

    auto writeTextFrame = [&frames](const QByteArray &id, const QString &text) {
        QByteArray textData = text.toUtf8();
        frames.append(id);
        quint32 size = 1 + textData.size() + 1;
        frames.append(char((size >> 24) & 0xFF));
        frames.append(char((size >> 16) & 0xFF));
        frames.append(char((size >> 8) & 0xFF));
        frames.append(char(size & 0xFF));
        frames.append(QByteArray(2, 0));
        frames.append(char(3)); // UTF-8
        frames.append(textData);
        frames.append(char(0));
    };

    if (!title.isEmpty()) writeTextFrame("TIT2", title);
    if (!artist.isEmpty()) writeTextFrame("TPE1", artist);
    if (!album.isEmpty()) writeTextFrame("TALB", album);

    if (!newCoverData.isEmpty()) {
        frames.append("APIC");
        QByteArray mimeType = "image/jpeg";
        quint32 size = 1 + mimeType.size() + 1 + 1 + 1 + newCoverData.size();
        frames.append(char((size >> 24) & 0xFF));
        frames.append(char((size >> 16) & 0xFF));
        frames.append(char((size >> 8) & 0xFF));
        frames.append(char(size & 0xFF));
        frames.append(QByteArray(2, 0));
        frames.append(char(0)); // ISO-8859-1 for mime string
        frames.append(mimeType);
        frames.append(char(0));
        frames.append(char(3)); // Cover (front)
        frames.append(char(0)); // Description empty
        frames.append(newCoverData);
    }

    QByteArray oldAudioData = fileData;
    if (fileData.size() > 10 && strncmp(fileData.constData(), "ID3", 3) == 0) {
        int majorVersion = fileData[3];
        quint32 oldTagSize = ((fileData[6] & 0x7F) << 21) |
                             ((fileData[7] & 0x7F) << 14) |
                             ((fileData[8] & 0x7F) << 7)  |
                             (fileData[9] & 0x7F);
        
        QByteArray tagData = fileData.mid(10, oldTagSize);
        oldAudioData = fileData.mid(10 + oldTagSize);

        int offset = 0;
        // Un-synch logic skipped for simplicity, most modern tags don't use it or we just copy raw bytes safely
        while (offset + (majorVersion == 2 ? 6 : 10) < tagData.size()) {
            const char *frameHeader = tagData.constData() + offset;
            QByteArray frameId;
            quint32 frameSize = 0;
            int headerSize = 0;

            if (majorVersion == 2) {
                frameId = QByteArray(frameHeader, 3);
                frameSize = (static_cast<uchar>(frameHeader[3]) << 16) |
                            (static_cast<uchar>(frameHeader[4]) << 8)  |
                            static_cast<uchar>(frameHeader[5]);
                headerSize = 6;
            } else {
                frameId = QByteArray(frameHeader, 4);
                if (majorVersion == 4) {
                    frameSize = ((frameHeader[4] & 0x7F) << 21) |
                                ((frameHeader[5] & 0x7F) << 14) |
                                ((frameHeader[6] & 0x7F) << 7)  |
                                (frameHeader[7] & 0x7F);
                } else {
                    frameSize = (static_cast<uchar>(frameHeader[4]) << 24) |
                                (static_cast<uchar>(frameHeader[5]) << 16) |
                                (static_cast<uchar>(frameHeader[6]) << 8)  |
                                static_cast<uchar>(frameHeader[7]);
                }
                headerSize = 10;
            }

            if (frameSize == 0 || offset + headerSize + frameSize > tagData.size()) break;

            if (frameId != "TIT2" && frameId != "TPE1" && frameId != "TALB" &&
                frameId != "TT2" && frameId != "TP1" && frameId != "TAL") {
                if (newCoverData.isEmpty() || (frameId != "APIC" && frameId != "PIC")) {
                    // Re-encode frame header to v2.3 to be safe, or just raw copy
                    if (majorVersion == 3 || majorVersion == 4) {
                        // Raw copy works mostly between 3 and 4
                        frames.append(tagData.mid(offset, headerSize + frameSize));
                    }
                }
            }
            offset += headerSize + frameSize;
        }
    }

    quint32 tagSize = frames.size();
    quint32 synchSize = ((tagSize & 0xFE00000) >> 21) |
                        ((tagSize & 0x1FC000) >> 14) |
                        ((tagSize & 0x3F80) >> 7) |
                        (tagSize & 0x7F);

    newTag.append(char((synchSize >> 21) & 0x7F));
    newTag.append(char((synchSize >> 14) & 0x7F));
    newTag.append(char((synchSize >> 7) & 0x7F));
    newTag.append(char(synchSize & 0x7F));
    newTag.append(frames);

    if (!file.open(QIODevice::WriteOnly)) return;
    file.write(newTag);
    file.write(oldAudioData);
    file.close();
}

void LocalLibrary::renameTrack(const QString &filePath, const QString &newTitle)
{
    for (int i = 0; i < m_tracks.size(); ++i) {
        if (m_tracks[i].filePath == filePath) {
            m_tracks[i].title = newTitle;
            writeId3Tag(filePath, newTitle, m_tracks[i].artist, m_tracks[i].album);

            // Physically rename the file on disk
            QFileInfo info(filePath);
            QString dir = info.absolutePath();
            QString ext = info.suffix();

            // Construct new file name based on artist and new title
            QString safeArtist = m_tracks[i].artist;
            QString safeTitle = newTitle;
            // Clean up invalid filesystem characters for Windows
            QStringList invalidChars = {"<", ">", ":", "\"", "/", "\\", "|", "?", "*"};
            for (const QString &c : invalidChars) {
                safeArtist.replace(c, "");
                safeTitle.replace(c, "");
            }

            QString newFileName = safeArtist + " - " + safeTitle + "." + ext;
            QString newPath = dir + "/" + newFileName;

            if (newPath != filePath && QFile::rename(filePath, newPath)) {
                // Update file path in memory
                m_tracks[i].filePath = newPath;

                // Update all custom playlists referencing the old file path
                bool playlistChanged = false;
                for (int pIdx = 0; pIdx < m_playlists.size(); ++pIdx) {
                    for (int tIdx = 0; tIdx < m_playlists[pIdx].trackPaths.size(); ++tIdx) {
                        if (m_playlists[pIdx].trackPaths[tIdx] == filePath) {
                            m_playlists[pIdx].trackPaths[tIdx] = newPath;
                            playlistChanged = true;
                        }
                    }
                }
                if (playlistChanged) savePlaylists();
            }
            emit libraryChanged();
            break;
        }
    }
}

void LocalLibrary::changeCover(const QString &filePath, const QString &imagePath)
{
    QString cleanPath = imagePath;
    if (cleanPath.startsWith("file:///")) {
        cleanPath = cleanPath.mid(8);
    }

    QFile imgFile(cleanPath);
    if (!imgFile.open(QIODevice::ReadOnly)) return;
    QByteArray imgData = imgFile.readAll();
    imgFile.close();

    for (int i = 0; i < m_tracks.size(); ++i) {
        if (m_tracks[i].filePath == filePath) {
            writeId3Tag(filePath, m_tracks[i].title, m_tracks[i].artist, m_tracks[i].album, imgData);
            m_tracks[i].hasCover = true;
            emit libraryChanged();
            break;
        }
    }
}


QByteArray LocalLibrary::getCoverData(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return QByteArray();

    char header[10];
    if (file.read(header, 10) == 10 && strncmp(header, "ID3", 3) == 0) {
        int majorVersion = header[3];
        quint32 tagSize = ((header[6] & 0x7F) << 21) |
                          ((header[7] & 0x7F) << 14) |
                          ((header[8] & 0x7F) << 7)  |
                          (header[9] & 0x7F);

        QByteArray tagData = file.read(tagSize);
        file.close();

        if ((header[5] & 0x80) != 0) {
            QByteArray decoded;
            decoded.reserve(tagData.size());
            for (int i = 0; i < tagData.size(); ++i) {
                decoded.append(tagData[i]);
                if (i + 1 < tagData.size() && static_cast<uchar>(tagData[i]) == 0xFF && static_cast<uchar>(tagData[i+1]) == 0x00) {
                    i++;
                }
            }
            tagData = decoded;
        }

        int tagOffset = 0;
        if ((header[5] & 0x40) != 0 && tagData.size() >= 4) {
            quint32 extHeaderSize = 0;
            if (majorVersion == 4) {
                extHeaderSize = ((tagData[0] & 0x7F) << 21) |
                                ((tagData[1] & 0x7F) << 14) |
                                ((tagData[2] & 0x7F) << 7)  |
                                (tagData[3] & 0x7F);
                tagOffset += extHeaderSize;
            } else {
                extHeaderSize = (static_cast<uchar>(tagData[0]) << 24) |
                                (static_cast<uchar>(tagData[1]) << 16) |
                                (static_cast<uchar>(tagData[2]) << 8)  |
                                static_cast<uchar>(tagData[3]);
                tagOffset += 4 + extHeaderSize;
            }
        }

        int offset = tagOffset;
        while (offset + (majorVersion == 2 ? 6 : 10) < tagData.size()) {
            const char *frameHeader = tagData.constData() + offset;
            QByteArray frameId;
            quint32 frameSize = 0;
            int headerSize = 0;

            if (majorVersion == 2) {
                frameId = QByteArray(frameHeader, 3);
                frameSize = (static_cast<uchar>(frameHeader[3]) << 16) |
                            (static_cast<uchar>(frameHeader[4]) << 8)  |
                            static_cast<uchar>(frameHeader[5]);
                headerSize = 6;
            } else {
                frameId = QByteArray(frameHeader, 4);
                if (majorVersion == 4) {
                    frameSize = ((frameHeader[4] & 0x7F) << 21) |
                                ((frameHeader[5] & 0x7F) << 14) |
                                ((frameHeader[6] & 0x7F) << 7)  |
                                (frameHeader[7] & 0x7F);
                } else {
                    frameSize = (static_cast<uchar>(frameHeader[4]) << 24) |
                                (static_cast<uchar>(frameHeader[5]) << 16) |
                                (static_cast<uchar>(frameHeader[6]) << 8)  |
                                static_cast<uchar>(frameHeader[7]);
                }
                headerSize = 10;
            }

            if (frameId.trimmed().isEmpty()) break;

            offset += headerSize;
            if (offset + frameSize > static_cast<quint32>(tagData.size())) break;

            if (frameId == "APIC" || frameId == "PIC") {
                const char *frameData = tagData.constData() + offset;

                int imgOffset = -1;
                for (int i = 1; i < static_cast<int>(frameSize) - 4; ++i) {
                    if (static_cast<uchar>(frameData[i]) == 0xFF && static_cast<uchar>(frameData[i+1]) == 0xD8) {
                        imgOffset = i;
                        break;
                    }
                    if (static_cast<uchar>(frameData[i]) == 0x89 &&
                        frameData[i+1] == 'P' &&
                        frameData[i+2] == 'N' &&
                        frameData[i+3] == 'G') {
                        imgOffset = i;
                        break;
                    }
                    if (frameData[i] == 'R' && frameData[i+1] == 'I' && frameData[i+2] == 'F' && frameData[i+3] == 'F') {
                        imgOffset = i;
                        break;
                    }
                    if (frameData[i] == 'G' && frameData[i+1] == 'I' && frameData[i+2] == 'F' && frameData[i+3] == '8') {
                        imgOffset = i;
                        break;
                    }
                }

                if (imgOffset != -1 && imgOffset < static_cast<int>(frameSize)) {
                    return QByteArray(frameData + imgOffset, frameSize - imgOffset);
                }

                int apicOffset = 0;
                if (majorVersion == 2) {
                    if (frameSize > 6) {
                        uchar encoding = frameData[apicOffset++];
                        apicOffset += 3;
                        apicOffset++;

                        if (encoding == 1) {
                            while (apicOffset + 1 < static_cast<int>(frameSize) && !(frameData[apicOffset] == 0 && frameData[apicOffset+1] == 0)) {
                                apicOffset++;
                            }
                            apicOffset += 2;
                        } else {
                            while (apicOffset < static_cast<int>(frameSize) && frameData[apicOffset] != 0) {
                                apicOffset++;
                            }
                            apicOffset++;
                        }

                        if (apicOffset < static_cast<int>(frameSize)) {
                            return QByteArray(frameData + apicOffset, frameSize - apicOffset);
                        }
                    }
                } else {
                    uchar encoding = frameData[apicOffset++];

                    while (apicOffset < static_cast<int>(frameSize) && frameData[apicOffset] != 0) {
                        apicOffset++;
                    }
                    apicOffset++;

                    if (apicOffset >= static_cast<int>(frameSize)) break;
                    apicOffset++;

                    if (encoding == 1 || encoding == 2) {
                        while (apicOffset + 1 < static_cast<int>(frameSize) && !(frameData[apicOffset] == 0 && frameData[apicOffset+1] == 0)) {
                            apicOffset++;
                        }
                        apicOffset += 2;
                    } else {
                        while (apicOffset < static_cast<int>(frameSize) && frameData[apicOffset] != 0) {
                            apicOffset++;
                        }
                        apicOffset++;
                    }

                    if (apicOffset < static_cast<int>(frameSize)) {
                        return QByteArray(frameData + apicOffset, frameSize - apicOffset);
                    }
                }
            }
            offset += frameSize;
        }
    }
    return QByteArray();
}

void LocalLibrary::loadPlaylists()
{
    m_playlists.clear();

    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(appData);
    QString path = appData + "/playlists.json";

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        m_defaultPlaylistName = "Library";
        m_defaultPlaylistCover = "";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject obj = doc.object();
    m_defaultPlaylistName = obj.value("defaultName").toString("Library");
    m_defaultPlaylistCover = obj.value("defaultCover").toString("");

    QJsonArray arr = obj.value("playlists").toArray();
    for (const QJsonValue &val : arr) {
        QJsonObject pObj = val.toObject();
        Playlist p;
        p.id = pObj.value("id").toString();
        p.name = pObj.value("name").toString();
        p.coverPath = pObj.value("coverPath").toString();
        p.dateCreated = QDateTime::fromString(pObj.value("dateCreated").toString(), Qt::ISODate);
        if (!p.dateCreated.isValid()) p.dateCreated = QDateTime::currentDateTime();

        QJsonArray tArr = pObj.value("tracks").toArray();
        for (const QJsonValue &tVal : tArr) {
            p.trackPaths.append(tVal.toString());
        }
        m_playlists.append(p);
    }
}

void LocalLibrary::savePlaylists()
{
    QString appData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString path = appData + "/playlists.json";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) return;

    QJsonObject obj;
    obj["defaultName"] = m_defaultPlaylistName;
    obj["defaultCover"] = m_defaultPlaylistCover;

    QJsonArray arr;
    for (const Playlist &p : m_playlists) {
        QJsonObject pObj;
        pObj["id"] = p.id;
        pObj["name"] = p.name;
        pObj["coverPath"] = p.coverPath;
        pObj["dateCreated"] = p.dateCreated.toString(Qt::ISODate);

        QJsonArray tArr;
        for (const QString &tPath : p.trackPaths) {
            tArr.append(tPath);
        }
        pObj["tracks"] = tArr;

        arr.append(pObj);
    }
    obj["playlists"] = arr;

    file.write(QJsonDocument(obj).toJson());
    file.close();
}

void LocalLibrary::createPlaylist(const QString &name, const QString &coverPath)
{
    Playlist p;
    p.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    p.name = name.isEmpty() ? "New Playlist" : name;
    p.coverPath = coverPath;
    p.dateCreated = QDateTime::currentDateTime();
    m_playlists.append(p);
    savePlaylists();
    emit playlistsChanged();
}

void LocalLibrary::deletePlaylist(const QString &id)
{
    for (int i = 0; i < m_playlists.size(); ++i) {
        if (m_playlists[i].id == id) {
            m_playlists.removeAt(i);
            break;
        }
    }
    savePlaylists();
    emit playlistsChanged();
}

void LocalLibrary::renamePlaylist(const QString &id, const QString &name)
{
    if (id == "default") {
        m_defaultPlaylistName = name;
        savePlaylists();
        emit playlistsChanged();
        return;
    }

    for (int i = 0; i < m_playlists.size(); ++i) {
        if (m_playlists[i].id == id) {
            m_playlists[i].name = name;
            break;
        }
    }
    savePlaylists();
    emit playlistsChanged();
}

void LocalLibrary::updatePlaylistCover(const QString &id, const QString &coverPath)
{
    if (id == "default") {
        m_defaultPlaylistCover = coverPath;
        savePlaylists();
        emit playlistsChanged();
        return;
    }

    for (int i = 0; i < m_playlists.size(); ++i) {
        if (m_playlists[i].id == id) {
            m_playlists[i].coverPath = coverPath;
            break;
        }
    }
    savePlaylists();
    emit playlistsChanged();
}

void LocalLibrary::addTrackToPlaylist(const QString &playlistId, const QString &filePath)
{
    if (playlistId == "default") return;

    for (int i = 0; i < m_playlists.size(); ++i) {
        if (m_playlists[i].id == playlistId) {
            if (!m_playlists[i].trackPaths.contains(filePath)) {
                m_playlists[i].trackPaths.append(filePath);
                savePlaylists();
                emit playlistsChanged();
            }
            break;
        }
    }
}

void LocalLibrary::removeTrackFromPlaylist(const QString &playlistId, const QString &filePath)
{
    if (playlistId == "default") return;

    for (int i = 0; i < m_playlists.size(); ++i) {
        if (m_playlists[i].id == playlistId) {
            m_playlists[i].trackPaths.removeAll(filePath);
            savePlaylists();
            emit playlistsChanged();
            break;
        }
    }
}

void LocalLibrary::createPlaylistFromMigration(const QString &playlistName, const QStringList &filePaths)
{
    Playlist p;
    p.id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    p.name = playlistName;
    p.trackPaths = filePaths;
    p.dateCreated = QDateTime::currentDateTime();
    m_playlists.append(p);
    savePlaylists();
    emit playlistsChanged();
}
