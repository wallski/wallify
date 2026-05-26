/****************************************************************************
** Meta object code from reading C++ file 'LocalLibrary.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/LocalLibrary.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LocalLibrary.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12LocalLibraryE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN12LocalLibraryE = QtMocHelpers::stringData(
    "LocalLibrary",
    "libraryChanged",
    "",
    "libraryPathChanged",
    "playlistsChanged",
    "sortModeChanged",
    "scan",
    "createPlaylist",
    "name",
    "coverPath",
    "deletePlaylist",
    "id",
    "renamePlaylist",
    "updatePlaylistCover",
    "addTrackToPlaylist",
    "playlistId",
    "filePath",
    "removeTrackFromPlaylist",
    "renameTrack",
    "newTitle",
    "changeCover",
    "imagePath",
    "setSortMode",
    "mode",
    "tracks",
    "QVariantList",
    "libraryPath",
    "playlists",
    "sortMode"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN12LocalLibraryE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       4,  153, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  104,    2, 0x06,    5 /* Public */,
       3,    0,  105,    2, 0x06,    6 /* Public */,
       4,    0,  106,    2, 0x06,    7 /* Public */,
       5,    0,  107,    2, 0x06,    8 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,  108,    2, 0x02,    9 /* Public */,
       7,    2,  109,    2, 0x02,   10 /* Public */,
       7,    1,  114,    2, 0x22,   13 /* Public | MethodCloned */,
      10,    1,  117,    2, 0x02,   15 /* Public */,
      12,    2,  120,    2, 0x02,   17 /* Public */,
      13,    2,  125,    2, 0x02,   20 /* Public */,
      14,    2,  130,    2, 0x02,   23 /* Public */,
      17,    2,  135,    2, 0x02,   26 /* Public */,
      18,    2,  140,    2, 0x02,   29 /* Public */,
      20,    2,  145,    2, 0x02,   32 /* Public */,
      22,    1,  150,    2, 0x02,   35 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   11,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   16,   21,
    QMetaType::Void, QMetaType::Int,   23,

 // properties: name, type, flags, notifyId, revision
      24, 0x80000000 | 25, 0x00015009, uint(0), 0,
      26, QMetaType::QString, 0x00015103, uint(1), 0,
      27, 0x80000000 | 25, 0x00015009, uint(2), 0,
      28, QMetaType::Int, 0x00015103, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject LocalLibrary::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN12LocalLibraryE.offsetsAndSizes,
    qt_meta_data_ZN12LocalLibraryE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN12LocalLibraryE_t,
        // property 'tracks'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'libraryPath'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'playlists'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'sortMode'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LocalLibrary, std::true_type>,
        // method 'libraryChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'libraryPathChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playlistsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sortModeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'scan'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'createPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'deletePlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'renamePlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'updatePlaylistCover'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'addTrackToPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'removeTrackFromPlaylist'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'renameTrack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'changeCover'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'setSortMode'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void LocalLibrary::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LocalLibrary *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->libraryChanged(); break;
        case 1: _t->libraryPathChanged(); break;
        case 2: _t->playlistsChanged(); break;
        case 3: _t->sortModeChanged(); break;
        case 4: _t->scan(); break;
        case 5: _t->createPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 6: _t->createPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->deletePlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->renamePlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->updatePlaylistCover((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->addTrackToPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->removeTrackFromPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 12: _t->renameTrack((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->changeCover((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 14: _t->setSortMode((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (LocalLibrary::*)();
            if (_q_method_type _q_method = &LocalLibrary::libraryChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (LocalLibrary::*)();
            if (_q_method_type _q_method = &LocalLibrary::libraryPathChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (LocalLibrary::*)();
            if (_q_method_type _q_method = &LocalLibrary::playlistsChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (LocalLibrary::*)();
            if (_q_method_type _q_method = &LocalLibrary::sortModeChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantList*>(_v) = _t->tracks(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->libraryPath(); break;
        case 2: *reinterpret_cast< QVariantList*>(_v) = _t->playlists(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->sortMode(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setLibraryPath(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->setSortMode(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *LocalLibrary::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LocalLibrary::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN12LocalLibraryE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LocalLibrary::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void LocalLibrary::libraryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void LocalLibrary::libraryPathChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LocalLibrary::playlistsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LocalLibrary::sortModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
