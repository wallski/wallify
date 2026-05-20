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

LocalLibrary::LocalLibrary(QObject *parent)
    : QObject(parent)
{
    QSettings settings("Wallski", "Wallify");
    m_libraryPath = settings.value("libraryPath", QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/Wallify").toString();
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
        list.append(map);
    }
    return list;
}

QVariantList LocalLibrary::playlists() const
{
    QVariantList list;

    // Add default playlist
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
        defTracks.append(tMap);
    }
    defMap["tracks"] = defTracks;
    list.append(defMap);

    // Add custom playlists
    for (const Playlist &p : m_playlists) {
        QVariantMap pMap;
        pMap["id"] = p.id;
        pMap["name"] = p.name;
        pMap["coverPath"] = p.coverPath;
        pMap["isDefault"] = false;

        QVariantList pTracks;
        for (const QString &path : p.trackPaths) {
            // Find track details from current scanned list
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
                    pTracks.append(tMap);
                    found = true;
                    break;
                }
            }
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
        m_tracks.append(parseMp3Metadata(filePath));
    }
    emit libraryChanged();
    emit playlistsChanged();
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
            
            // Bitrate mapping for Layer 3 (in kbps)
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
    
    // Default fallback values based on file name
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

    // Parse ID3v2 tags
    QFile file(filePath);
    qint64 totalId3Size = 0;
    if (file.open(QIODevice::ReadOnly)) {
        char header[10];
        if (file.read(header, 10) == 10 && strncmp(header, "ID3", 3) == 0) {
            int majorVersion = header[3];
            
            // Read tag size (synchsafe integer: 4 bytes, 7 bits per byte)
            quint32 tagSize = ((header[6] & 0x7F) << 21) |
                              ((header[7] & 0x7F) << 14) |
                              ((header[8] & 0x7F) << 7)  |
                              (header[9] & 0x7F);
            
            totalId3Size = 10 + tagSize;
            QByteArray tagData = file.read(tagSize);
            
            // Handle unsynchronization on the entire tag
            if ((header[5] & 0x80) != 0) {
                QByteArray decoded;
                decoded.reserve(tagData.size());
                for (int i = 0; i < tagData.size(); ++i) {
                    decoded.append(tagData[i]);
                    if (i + 1 < tagData.size() && static_cast<uchar>(tagData[i]) == 0xFF && static_cast<uchar>(tagData[i+1]) == 0x00) {
                        i++; // Skip the unsync zero byte
                    }
                }
                tagData = decoded;
            }

            // Handle extended header (only if present)
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

    // Estimate duration if not parsed from TLEN/TLE
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
    
    // Strip trailing/leading zeros or padding
    while (textSize > 0 && textData[textSize - 1] == 0) {
        textSize--;
    }

    if (encoding == 0) {
        return QString::fromLatin1(textData, textSize).trimmed();
    } else if (encoding == 1) {
        // UTF-16 with BOM
        if (textSize >= 2) {
            // Check BOM (0xFFFE or 0xFEFF)
            const uchar *bom = reinterpret_cast<const uchar*>(textData);
            if ((bom[0] == 0xFF && bom[1] == 0xFE) || (bom[0] == 0xFE && bom[1] == 0xFF)) {
                // Skip BOM
                textData += 2;
                textSize -= 2;
            }
        }
        return QString::fromUtf16(reinterpret_cast<const char16_t*>(textData), textSize / 2).trimmed();
    } else if (encoding == 2) {
        // UTF-16BE without BOM
        return QString::fromUtf16(reinterpret_cast<const char16_t*>(textData), textSize / 2).trimmed();
    } else if (encoding == 3) {
        // UTF-8
        return QString::fromUtf8(textData, textSize).trimmed();
    }
    return QString::fromLatin1(textData, textSize).trimmed();
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

        // Handle unsynchronization on the entire tag
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

        // Handle extended header (only if present)
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
                
                // Scan the frame data for image magic byte signatures (JPEG, PNG, WebP, GIF)
                int imgOffset = -1;
                for (int i = 1; i < static_cast<int>(frameSize) - 4; ++i) {
                    // Check for JPEG (FF D8)
                    if (static_cast<uchar>(frameData[i]) == 0xFF && static_cast<uchar>(frameData[i+1]) == 0xD8) {
                        imgOffset = i;
                        break;
                    }
                    // Check for PNG (89 50 4E 47)
                    if (static_cast<uchar>(frameData[i]) == 0x89 &&
                        frameData[i+1] == 'P' &&
                        frameData[i+2] == 'N' &&
                        frameData[i+3] == 'G') {
                        imgOffset = i;
                        break;
                    }
                    // Check for WebP / RIFF (52 49 46 46)
                    if (frameData[i] == 'R' && frameData[i+1] == 'I' && frameData[i+2] == 'F' && frameData[i+3] == 'F') {
                        imgOffset = i;
                        break;
                    }
                    // Check for GIF (47 49 46 38)
                    if (frameData[i] == 'G' && frameData[i+1] == 'I' && frameData[i+2] == 'F' && frameData[i+3] == '8') {
                        imgOffset = i;
                        break;
                    }
                }
                
                if (imgOffset != -1 && imgOffset < static_cast<int>(frameSize)) {
                    qDebug() << "LocalLibrary: Found cover image at offset" << imgOffset << "within APIC frame of size" << frameSize;
                    return QByteArray(frameData + imgOffset, frameSize - imgOffset);
                }
                
                // Fallback: If we couldn't find magic bytes, use standard fallback parsing
                int apicOffset = 0;
                if (majorVersion == 2) {
                    if (frameSize > 6) {
                        uchar encoding = frameData[apicOffset++];
                        apicOffset += 3; // Skip image format (3 chars)
                        apicOffset++; // Skip picture type (1 byte)
                        
                        // Skip Description
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
                    
                    // Read MIME type
                    while (apicOffset < static_cast<int>(frameSize) && frameData[apicOffset] != 0) {
                        apicOffset++;
                    }
                    apicOffset++; // Skip null terminator
                    
                    if (apicOffset >= static_cast<int>(frameSize)) break;
                    apicOffset++; // Skip picture type (1 byte)
                    
                    // Skip Description
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
        // Setup default configuration
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
    if (playlistId == "default") return; // Can't add explicitly to default

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
    if (playlistId == "default") return; // Can't remove from default

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
    m_playlists.append(p);
    savePlaylists();
    emit playlistsChanged();
}
