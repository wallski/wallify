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
    "tracks",
    "QVariantList",
    "libraryPath",
    "playlists"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN12LocalLibraryE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       3,  115, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x06,    4 /* Public */,
       3,    0,   81,    2, 0x06,    5 /* Public */,
       4,    0,   82,    2, 0x06,    6 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   83,    2, 0x02,    7 /* Public */,
       6,    2,   84,    2, 0x02,    8 /* Public */,
       6,    1,   89,    2, 0x22,   11 /* Public | MethodCloned */,
       9,    1,   92,    2, 0x02,   13 /* Public */,
      11,    2,   95,    2, 0x02,   15 /* Public */,
      12,    2,  100,    2, 0x02,   18 /* Public */,
      13,    2,  105,    2, 0x02,   21 /* Public */,
      16,    2,  110,    2, 0x02,   24 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   10,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   14,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   14,   15,

 // properties: name, type, flags, notifyId, revision
      17, 0x80000000 | 18, 0x00015009, uint(0), 0,
      19, QMetaType::QString, 0x00015103, uint(1), 0,
      20, 0x80000000 | 18, 0x00015009, uint(2), 0,

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
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LocalLibrary, std::true_type>,
        // method 'libraryChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'libraryPathChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'playlistsChanged'
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
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
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
        case 3: _t->scan(); break;
        case 4: _t->createPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->createPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->deletePlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->renamePlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 8: _t->updatePlaylistCover((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 9: _t->addTrackToPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->removeTrackFromPlaylist((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
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
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantList*>(_v) = _t->tracks(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->libraryPath(); break;
        case 2: *reinterpret_cast< QVariantList*>(_v) = _t->playlists(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setLibraryPath(*reinterpret_cast< QString*>(_v)); break;
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
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
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
QT_WARNING_POP
