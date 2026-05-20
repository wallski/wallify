#include "AudioPlayer.h"
#include <QUrl>
#include <QDebug>
#include <QTimer>

AudioPlayer::AudioPlayer(LocalLibrary *library, QObject *parent)
    : QObject(parent), m_library(library), m_isPlaying(false), m_loopMode(0)
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    // Initial volume 70%
    m_audioOutput->setVolume(0.7);

    connect(m_player, &QMediaPlayer::positionChanged, this, &AudioPlayer::handlePositionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &AudioPlayer::handleDurationChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &AudioPlayer::handlePlaybackStateChanged);
    
    // Handle error events
    connect(m_player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
        qWarning() << "AudioPlayer: QMediaPlayer error occurred:" << error << "-" << errorString;
    });

    // Auto-advance on track finished and log media status
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        qDebug() << "AudioPlayer: MediaStatus changed to:" << status;
        if (status == QMediaPlayer::EndOfMedia) {
            if (m_loopMode == 2) {
                m_player->setPosition(0);
                m_player->play();
            } else {
                next();
            }
        }
    });
}

bool AudioPlayer::isPlaying() const
{
    return m_isPlaying;
}

int AudioPlayer::position() const
{
    return static_cast<int>(m_player->position());
}

void AudioPlayer::setPosition(int pos)
{
    if (m_player->position() != pos) {
        m_isSeeking = true;
        m_player->setPosition(pos);
        emit positionChanged();
        
        QTimer::singleShot(500, this, [this]() {
            m_isSeeking = false;
            emit positionChanged();
        });
    }
}

int AudioPlayer::duration() const
{
    return static_cast<int>(m_player->duration());
}

double AudioPlayer::volume() const
{
    return m_volume;
}

void AudioPlayer::setVolume(double vol)
{
    if (m_volume != vol) {
        m_volume = vol;
        m_audioOutput->setVolume(static_cast<float>(vol));
        emit volumeChanged();
    }
}

QVariantMap AudioPlayer::currentTrack() const
{
    return m_currentTrack;
}

void AudioPlayer::play()
{
    if (!m_currentTrack.isEmpty()) {
        m_player->play();
    }
}

void AudioPlayer::pause()
{
    m_player->pause();
}

void AudioPlayer::stop()
{
    m_player->stop();
}

void AudioPlayer::playTrack(const QVariantMap &track)
{
    m_currentTrack = track;
    emit currentTrackChanged();

    if (m_playlistQueue.isEmpty()) {
        m_playlistQueue.append(track);
    }

    QString path = track["filePath"].toString();
    qDebug() << "AudioPlayer: playTrack requested for:" << path;
    m_player->setSource(QUrl::fromLocalFile(path));
    m_player->play();
}

void AudioPlayer::playTrackFromPlaylist(const QVariantMap &track, const QVariantList &playlistTracks)
{
    m_playlistQueue = playlistTracks;
    playTrack(track);
}

void AudioPlayer::next()
{
    // 1. Process user queue first
    if (!m_playQueue.isEmpty()) {
        QVariantMap nextTrack = m_playQueue.takeFirst().toMap();
        emit playQueueChanged();
        
        // Ensure playTrack has playlist context if it was empty, or preserve it
        playTrack(nextTrack);
        return;
    }

    // 2. Play next in playlist context
    if (m_playlistQueue.isEmpty() || m_currentTrack.isEmpty()) return;

    QString currentPath = m_currentTrack["filePath"].toString();
    int currentIndex = -1;

    for (int i = 0; i < m_playlistQueue.size(); ++i) {
        if (m_playlistQueue[i].toMap()["filePath"].toString() == currentPath) {
            currentIndex = i;
            break;
        }
    }

    if (currentIndex != -1 && currentIndex < m_playlistQueue.size() - 1) {
        playTrack(m_playlistQueue[currentIndex + 1].toMap());
    } else {
        if (m_loopMode == 1) {
            // Repeat Playlist
            playTrack(m_playlistQueue[0].toMap());
        } else {
            // Repeat Off: Stop playback
            stop();
        }
    }
}

void AudioPlayer::previous()
{
    if (m_playlistQueue.isEmpty() || m_currentTrack.isEmpty()) return;

    QString currentPath = m_currentTrack["filePath"].toString();
    int currentIndex = -1;

    for (int i = 0; i < m_playlistQueue.size(); ++i) {
        if (m_playlistQueue[i].toMap()["filePath"].toString() == currentPath) {
            currentIndex = i;
            break;
        }
    }

    if (currentIndex > 0) {
        playTrack(m_playlistQueue[currentIndex - 1].toMap());
    } else if (m_playlistQueue.size() > 0) {
        if (m_loopMode == 1) {
            // Repeat Playlist: go to last
            playTrack(m_playlistQueue.last().toMap());
        } else {
            // Seek to 0
            setPosition(0);
        }
    }
}

void AudioPlayer::handlePositionChanged(qint64 position)
{
    Q_UNUSED(position);
    if (!m_isSeeking) {
        emit positionChanged();
    }
}

void AudioPlayer::handleDurationChanged(qint64 duration)
{
    Q_UNUSED(duration);
    emit durationChanged();
}

void AudioPlayer::handlePlaybackStateChanged()
{
    bool playing = (m_player->playbackState() == QMediaPlayer::PlayingState);
    if (m_isPlaying != playing) {
        m_isPlaying = playing;
        emit isPlayingChanged();
    }
}

void AudioPlayer::addToQueue(const QVariantMap &track)
{
    m_playQueue.append(track);
    emit playQueueChanged();
}

void AudioPlayer::clearQueue()
{
    m_playQueue.clear();
    emit playQueueChanged();
}

int AudioPlayer::loopMode() const
{
    return m_loopMode;
}

void AudioPlayer::setLoopMode(int mode)
{
    if (m_loopMode != mode) {
        m_loopMode = mode;
        emit loopModeChanged();
    }
}

QVariantList AudioPlayer::playQueue() const
{
    return m_playQueue;
}
