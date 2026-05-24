#include "AudioPlayer.h"
#include <QUrl>
#include <QDebug>
#include <QTimer>
#include <QDateTime>

AudioPlayer::AudioPlayer(LocalLibrary *library, QObject *parent)
    : QObject(parent), m_library(library), m_isPlaying(false), m_loopMode(0), m_shuffleEnabled(false)
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);
    m_audioOutput->setVolume(0.7);

    connect(m_player, &QMediaPlayer::positionChanged, this, &AudioPlayer::handlePositionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &AudioPlayer::handleDurationChanged);
    connect(m_player, &QMediaPlayer::playbackStateChanged, this, &AudioPlayer::handlePlaybackStateChanged);

    connect(m_player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
        qWarning() << "AudioPlayer: QMediaPlayer error occurred:" << error << "-" << errorString;
    });

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
    emit trackChanged(track);

    if (m_playlistQueue.isEmpty()) {
        m_playlistQueue.append(track);
    }

    if (m_shuffleEnabled && !m_shuffleOrder.isEmpty()) {
        QString path = track["filePath"].toString();
        for (int i = 0; i < m_shuffleOrder.size(); ++i) {
            int idx = m_shuffleOrder[i];
            if (idx >= 0 && idx < m_playlistQueue.size()) {
                if (m_playlistQueue[idx].toMap()["filePath"].toString() == path) {
                    m_shuffleCurrentIndex = i;
                    break;
                }
            }
        }
    }

    QString path = track["filePath"].toString();
    qDebug() << "AudioPlayer: playTrack requested for:" << path;
    m_player->setSource(QUrl::fromLocalFile(path));
    m_player->play();
}

void AudioPlayer::playTrackFromPlaylist(const QVariantMap &track, const QVariantList &playlistTracks)
{
    m_playlistQueue = playlistTracks;
    if (m_shuffleEnabled) {
        rebuildShuffleOrder();
    }
    playTrack(track);
}

void AudioPlayer::playPlaylist(const QVariantList &tracks, bool shuffle)
{
    if (tracks.isEmpty()) return;

    m_playlistQueue = tracks;
    m_shuffleEnabled = shuffle;
    emit shuffleEnabledChanged();

    if (m_shuffleEnabled) {
        rebuildShuffleOrder();
        playTrack(m_playlistQueue[m_shuffleOrder.first()].toMap());
    } else {
        playTrack(tracks.first().toMap());
    }
}

int AudioPlayer::getNextShuffledIndex()
{
    if (m_shuffleOrder.isEmpty()) return -1;
    m_shuffleCurrentIndex++;
    if (m_shuffleCurrentIndex >= m_shuffleOrder.size()) {
        if (m_loopMode == 1) {
            m_shuffleCurrentIndex = 0;
        } else {
            return -1;
        }
    }
    return m_shuffleOrder[m_shuffleCurrentIndex];
}

int AudioPlayer::getPreviousShuffledIndex()
{
    if (m_shuffleOrder.isEmpty()) return -1;
    m_shuffleCurrentIndex--;
    if (m_shuffleCurrentIndex < 0) {
        if (m_loopMode == 1) {
            m_shuffleCurrentIndex = m_shuffleOrder.size() - 1;
        } else {
            m_shuffleCurrentIndex = 0;
            return -1;
        }
    }
    return m_shuffleOrder[m_shuffleCurrentIndex];
}

void AudioPlayer::rebuildShuffleOrder()
{
    m_shuffleOrder.clear();
    for (int i = 0; i < m_playlistQueue.size(); ++i) {
        m_shuffleOrder.append(i);
    }

    QRandomGenerator *rng = QRandomGenerator::global();
    for (int i = m_shuffleOrder.size() - 1; i > 0; --i) {
        int j = rng->bounded(i + 1);
        std::swap(m_shuffleOrder[i], m_shuffleOrder[j]);
    }

    if (!m_currentTrack.isEmpty()) {
        QString currentPath = m_currentTrack["filePath"].toString();
        for (int i = 0; i < m_shuffleOrder.size(); ++i) {
            if (m_playlistQueue[m_shuffleOrder[i]].toMap()["filePath"].toString() == currentPath) {
                m_shuffleCurrentIndex = i;
                break;
            }
        }
    } else {
        m_shuffleCurrentIndex = 0;
    }
}

void AudioPlayer::next()
{
    if (!m_playQueue.isEmpty()) {
        QVariantMap nextTrack = m_playQueue.takeFirst().toMap();
        emit playQueueChanged();
        playTrack(nextTrack);
        return;
    }

    if (m_playlistQueue.isEmpty() || m_currentTrack.isEmpty()) return;

    if (m_shuffleEnabled) {
        int nextIdx = getNextShuffledIndex();
        if (nextIdx != -1) {
            playTrack(m_playlistQueue[nextIdx].toMap());
        } else {
            stop();
        }
        return;
    }

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
            playTrack(m_playlistQueue[0].toMap());
        } else {
            stop();
        }
    }
}

void AudioPlayer::previous()
{
    if (m_playlistQueue.isEmpty() || m_currentTrack.isEmpty()) return;

    if (m_shuffleEnabled) {
        int prevIdx = getPreviousShuffledIndex();
        if (prevIdx != -1) {
            playTrack(m_playlistQueue[prevIdx].toMap());
        }
        return;
    }

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
            playTrack(m_playlistQueue.last().toMap());
        } else {
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

bool AudioPlayer::shuffleEnabled() const
{
    return m_shuffleEnabled;
}

void AudioPlayer::setShuffleEnabled(bool enabled)
{
    if (m_shuffleEnabled != enabled) {
        m_shuffleEnabled = enabled;
        if (m_shuffleEnabled && !m_playlistQueue.isEmpty()) {
            rebuildShuffleOrder();
        }
        emit shuffleEnabledChanged();
    }
}
