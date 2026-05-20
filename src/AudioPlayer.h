#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVariantMap>
#include "LocalLibrary.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY isPlayingChanged)
    Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(double volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(QVariantMap currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(int loopMode READ loopMode WRITE setLoopMode NOTIFY loopModeChanged)
    Q_PROPERTY(QVariantList playQueue READ playQueue NOTIFY playQueueChanged)

public:
    explicit AudioPlayer(LocalLibrary *library, QObject *parent = nullptr);

    bool isPlaying() const;
    int position() const;
    Q_INVOKABLE void setPosition(int pos);
    int duration() const;
    double volume() const;
    Q_INVOKABLE void setVolume(double vol);
    QVariantMap currentTrack() const;

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void playTrack(const QVariantMap &track);
    Q_INVOKABLE void playTrackFromPlaylist(const QVariantMap &track, const QVariantList &playlistTracks);
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();
    
    // Queue and Loop functions
    Q_INVOKABLE void addToQueue(const QVariantMap &track);
    Q_INVOKABLE void clearQueue();
    int loopMode() const;
    Q_INVOKABLE void setLoopMode(int mode);
    QVariantList playQueue() const;

signals:
    void isPlayingChanged();
    void positionChanged();
    void durationChanged();
    void volumeChanged();
    void currentTrackChanged();
    void loopModeChanged();
    void playQueueChanged();

private slots:
    void handlePositionChanged(qint64 position);
    void handleDurationChanged(qint64 duration);
    void handlePlaybackStateChanged();

private:
    LocalLibrary *m_library;
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QVariantMap m_currentTrack;
    bool m_isPlaying;
    int m_loopMode; // 0 = None, 1 = Repeat All, 2 = Repeat One
    QVariantList m_playQueue;       // User queue
    QVariantList m_playlistQueue;   // Playlist/scanned songs context queue
    bool m_isSeeking = false;
    double m_volume = 0.7;
};

#endif // AUDIOPLAYER_H
