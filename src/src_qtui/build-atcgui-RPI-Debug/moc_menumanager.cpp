/****************************************************************************
** Meta object code from reading C++ file 'menumanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../atcgui/menumanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menumanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MenuManager_t {
    QByteArrayData data[14];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MenuManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MenuManager_t qt_meta_stringdata_MenuManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "MenuManager"
QT_MOC_LITERAL(1, 12, 15), // "lb_settings_btn"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 11), // "lb_info_btn"
QT_MOC_LITERAL(4, 41, 12), // "ls_login_btn"
QT_MOC_LITERAL(5, 54, 15), // "mm_start_ai_btn"
QT_MOC_LITERAL(6, 70, 18), // "mm_player_list_btn"
QT_MOC_LITERAL(7, 89, 29), // "mm_search_for_players_toggled"
QT_MOC_LITERAL(8, 119, 6), // "_state"
QT_MOC_LITERAL(9, 126, 20), // "sm_open_settings_btn"
QT_MOC_LITERAL(10, 147, 11), // "sm_init_btn"
QT_MOC_LITERAL(11, 159, 13), // "sm_logout_btn"
QT_MOC_LITERAL(12, 173, 17), // "sm_scan_board_btn"
QT_MOC_LITERAL(13, 191, 12) // "go_menu_back"

    },
    "MenuManager\0lb_settings_btn\0\0lb_info_btn\0"
    "ls_login_btn\0mm_start_ai_btn\0"
    "mm_player_list_btn\0mm_search_for_players_toggled\0"
    "_state\0sm_open_settings_btn\0sm_init_btn\0"
    "sm_logout_btn\0sm_scan_board_btn\0"
    "go_menu_back"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MenuManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x0a /* Public */,
       3,    0,   70,    2, 0x0a /* Public */,
       4,    0,   71,    2, 0x0a /* Public */,
       5,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    1,   74,    2, 0x0a /* Public */,
       9,    0,   77,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,
      12,    0,   80,    2, 0x0a /* Public */,
      13,    0,   81,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MenuManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MenuManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->lb_settings_btn(); break;
        case 1: _t->lb_info_btn(); break;
        case 2: _t->ls_login_btn(); break;
        case 3: _t->mm_start_ai_btn(); break;
        case 4: _t->mm_player_list_btn(); break;
        case 5: _t->mm_search_for_players_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->sm_open_settings_btn(); break;
        case 7: _t->sm_init_btn(); break;
        case 8: _t->sm_logout_btn(); break;
        case 9: _t->sm_scan_board_btn(); break;
        case 10: _t->go_menu_back(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MenuManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MenuManager.data,
    qt_meta_data_MenuManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MenuManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MenuManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MenuManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MenuManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
