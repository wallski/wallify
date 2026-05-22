/****************************************************************************
** Meta object code from reading C++ file 'AudioPlayer.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AudioPlayer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioPlayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11AudioPlayerE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN11AudioPlayerE = QtMocHelpers::stringData(
    "AudioPlayer",
    "isPlayingChanged",
    "",
    "positionChanged",
    "durationChanged",
    "volumeChanged",
    "currentTrackChanged",
    "loopModeChanged",
    "playQueueChanged",
    "shuffleEnabledChanged",
    "trackChanged",
    "QVariantMap",
    "track",
    "handlePositionChanged",
    "position",
    "handleDurationChanged",
    "duration",
    "handlePlaybackStateChanged",
    "setPosition",
    "pos",
    "setVolume",
    "vol",
    "play",
    "pause",
    "stop",
    "playTrack",
    "playTrackFromPlaylist",
    "QVariantList",
    "playlistTracks",
    "next",
    "previous",
    "addToQueue",
    "clearQueue",
    "setLoopMode",
    "mode",
    "setShuffleEnabled",
    "enabled",
    "playPlaylist",
    "tracks",
    "shuffle",
    "isPlaying",
    "volume",
    "currentTrack",
    "loopMode",
    "playQueue",
    "shuffleEnabled"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN11AudioPlayerE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       8,  222, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  170,    2, 0x06,    9 /* Public */,
       3,    0,  171,    2, 0x06,   10 /* Public */,
       4,    0,  172,    2, 0x06,   11 /* Public */,
       5,    0,  173,    2, 0x06,   12 /* Public */,
       6,    0,  174,    2, 0x06,   13 /* Public */,
       7,    0,  175,    2, 0x06,   14 /* Public */,
       8,    0,  176,    2, 0x06,   15 /* Public */,
       9,    0,  177,    2, 0x06,   16 /* Public */,
      10,    1,  178,    2, 0x06,   17 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      13,    1,  181,    2, 0x08,   19 /* Private */,
      15,    1,  184,    2, 0x08,   21 /* Private */,
      17,    0,  187,    2, 0x08,   23 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      18,    1,  188,    2, 0x02,   24 /* Public */,
      20,    1,  191,    2, 0x02,   26 /* Public */,
      22,    0,  194,    2, 0x02,   28 /* Public */,
      23,    0,  195,    2, 0x02,   29 /* Public */,
      24,    0,  196,    2, 0x02,   30 /* Public */,
      25,    1,  197,    2, 0x02,   31 /* Public */,
      26,    2,  200,    2, 0x02,   33 /* Public */,
      29,    0,  205,    2, 0x02,   36 /* Public */,
      30,    0,  206,    2, 0x02,   37 /* Public */,
      31,    1,  207,    2, 0x02,   38 /* Public */,
      32,    0,  210,    2, 0x02,   40 /* Public */,
      33,    1,  211,    2, 0x02,   41 /* Public */,
      35,    1,  214,    2, 0x02,   43 /* Public */,
      37,    2,  217,    2, 0x02,   45 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::LongLong,   14,
    QMetaType::Void, QMetaType::LongLong,   16,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Double,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11, 0x80000000 | 27,   12,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   34,
    QMetaType::Void, QMetaType::Bool,   36,
    QMetaType::Void, 0x80000000 | 27, QMetaType::Bool,   38,   39,

 // properties: name, type, flags, notifyId, revision
      40, QMetaType::Bool, 0x00015001, uint(0), 0,
      14, QMetaType::Int, 0x00015103, uint(1), 0,
      16, QMetaType::Int, 0x00015001, uint(2), 0,
      41, QMetaType::Double, 0x00015103, uint(3), 0,
      42, 0x80000000 | 11, 0x00015009, uint(4), 0,
      43, QMetaType::Int, 0x00015103, uint(5), 0,
      44, 0x80000000 | 27, 0x00015009, uint(6), 0,
      45, QMetaType::Bool, 0x00015103, uint(7), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject AudioPlayer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN11AudioPlayerE.offsetsAndSizes,
    qt_meta_data_ZN11AudioPlayerE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN11AudioPlayerE_t,
        // property 'isPlaying'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'position'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'duration'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'volume'
        QtPrivate::TypeAndForceComplete<double, std::true_type>,
        // property 'currentTrack'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::true_type>,
        // property 'loopMode'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // property 'playQueue'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'shuffleEnabled'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AudioPlayer, std::true_type>,
        // method 'isPlayingChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'positionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'durationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'volumeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'currentTrackChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loopModeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playQueueChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'shuffleEnabledChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'trackChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'handlePositionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'handleDurationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<qint64, std::false_type>,
        // method 'handlePlaybackStateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setPosition'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setVolume'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'play'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pause'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stop'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playTrack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'playTrackFromPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantList &, std::false_type>,
        // method 'next'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'previous'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addToQueue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'clearQueue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setLoopMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setShuffleEnabled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'playPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantList &, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void AudioPlayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AudioPlayer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->isPlayingChanged(); break;
        case 1: _t->positionChanged(); break;
        case 2: _t->durationChanged(); break;
        case 3: _t->volumeChanged(); break;
        case 4: _t->currentTrackChanged(); break;
        case 5: _t->loopModeChanged(); break;
        case 6: _t->playQueueChanged(); break;
        case 7: _t->shuffleEnabledChanged(); break;
        case 8: _t->trackChanged((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 9: _t->handlePositionChanged((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 10: _t->handleDurationChanged((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 11: _t->handlePlaybackStateChanged(); break;
        case 12: _t->setPosition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setVolume((*reinterpret_cast< std::add_pointer_t<double>>(_a[1]))); break;
        case 14: _t->play(); break;
        case 15: _t->pause(); break;
        case 16: _t->stop(); break;
        case 17: _t->playTrack((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 18: _t->playTrackFromPlaylist((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[2]))); break;
        case 19: _t->next(); break;
        case 20: _t->previous(); break;
        case 21: _t->addToQueue((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 22: _t->clearQueue(); break;
        case 23: _t->setLoopMode((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->setShuffleEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 25: _t->playPlaylist((*reinterpret_cast< std::add_pointer_t<QVariantList>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::isPlayingChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::positionChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::durationChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::volumeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::currentTrackChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::loopModeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::playQueueChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)();
            if (_q_method_type _q_method = &AudioPlayer::shuffleEnabledChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (AudioPlayer::*)(const QVariantMap & );
            if (_q_method_type _q_method = &AudioPlayer::trackChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->isPlaying(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->position(); break;
        case 2: *reinterpret_cast< int*>(_v) = _t->duration(); break;
        case 3: *reinterpret_cast< double*>(_v) = _t->volume(); break;
        case 4: *reinterpret_cast< QVariantMap*>(_v) = _t->currentTrack(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->loopMode(); break;
        case 6: *reinterpret_cast< QVariantList*>(_v) = _t->playQueue(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->shuffleEnabled(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setPosition(*reinterpret_cast< int*>(_v)); break;
        case 3: _t->setVolume(*reinterpret_cast< double*>(_v)); break;
        case 5: _t->setLoopMode(*reinterpret_cast< int*>(_v)); break;
        case 7: _t->setShuffleEnabled(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *AudioPlayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioPlayer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN11AudioPlayerE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AudioPlayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 26;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AudioPlayer::isPlayingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AudioPlayer::positionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AudioPlayer::durationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AudioPlayer::volumeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void AudioPlayer::currentTrackChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void AudioPlayer::loopModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void AudioPlayer::playQueueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void AudioPlayer::shuffleEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void AudioPlayer::trackChanged(const QVariantMap & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
