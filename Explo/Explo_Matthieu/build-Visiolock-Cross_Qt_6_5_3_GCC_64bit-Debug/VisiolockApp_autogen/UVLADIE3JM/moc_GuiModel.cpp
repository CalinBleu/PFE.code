/****************************************************************************
** Meta object code from reading C++ file 'GuiModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Visiolock/src/GuiModel.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.1. It"
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

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSGuiModelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSGuiModelENDCLASS = QtMocHelpers::stringData(
    "GuiModel",
    "Gui_askConnexionController",
    "",
    "char*",
    "password",
    "Gui_changeModeController",
    "Mode",
    "mode",
    "Gui_userAddedController",
    "User",
    "user",
    "Gui_userModifiedController",
    "Gui_userRemovedController",
    "idTag",
    "Gui_userSearchedController",
    "searchField"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSGuiModelENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[9];
    char stringdata1[27];
    char stringdata2[1];
    char stringdata3[6];
    char stringdata4[9];
    char stringdata5[25];
    char stringdata6[5];
    char stringdata7[5];
    char stringdata8[24];
    char stringdata9[5];
    char stringdata10[5];
    char stringdata11[27];
    char stringdata12[26];
    char stringdata13[6];
    char stringdata14[27];
    char stringdata15[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSGuiModelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSGuiModelENDCLASS_t qt_meta_stringdata_CLASSGuiModelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 8),  // "GuiModel"
        QT_MOC_LITERAL(9, 26),  // "Gui_askConnexionController"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 5),  // "char*"
        QT_MOC_LITERAL(43, 8),  // "password"
        QT_MOC_LITERAL(52, 24),  // "Gui_changeModeController"
        QT_MOC_LITERAL(77, 4),  // "Mode"
        QT_MOC_LITERAL(82, 4),  // "mode"
        QT_MOC_LITERAL(87, 23),  // "Gui_userAddedController"
        QT_MOC_LITERAL(111, 4),  // "User"
        QT_MOC_LITERAL(116, 4),  // "user"
        QT_MOC_LITERAL(121, 26),  // "Gui_userModifiedController"
        QT_MOC_LITERAL(148, 25),  // "Gui_userRemovedController"
        QT_MOC_LITERAL(174, 5),  // "idTag"
        QT_MOC_LITERAL(180, 26),  // "Gui_userSearchedController"
        QT_MOC_LITERAL(207, 11)   // "searchField"
    },
    "GuiModel",
    "Gui_askConnexionController",
    "",
    "char*",
    "password",
    "Gui_changeModeController",
    "Mode",
    "mode",
    "Gui_userAddedController",
    "User",
    "user",
    "Gui_userModifiedController",
    "Gui_userRemovedController",
    "idTag",
    "Gui_userSearchedController",
    "searchField"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGuiModelENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x02,    1 /* Public */,
       5,    1,   53,    2, 0x02,    3 /* Public */,
       8,    1,   56,    2, 0x02,    5 /* Public */,
      11,    1,   59,    2, 0x02,    7 /* Public */,
      12,    1,   62,    2, 0x02,    9 /* Public */,
      14,    1,   65,    2, 0x02,   11 /* Public */,

 // methods: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 3,   13,
    QMetaType::Void, 0x80000000 | 3,   15,

       0        // eod
};

Q_CONSTINIT const QMetaObject GuiModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSGuiModelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGuiModelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSGuiModelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GuiModel, std::true_type>,
        // method 'Gui_askConnexionController'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<char *, std::false_type>,
        // method 'Gui_changeModeController'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<Mode, std::false_type>,
        // method 'Gui_userAddedController'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<User, std::false_type>,
        // method 'Gui_userModifiedController'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<User, std::false_type>,
        // method 'Gui_userRemovedController'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<char *, std::false_type>,
        // method 'Gui_userSearchedController'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<char *, std::false_type>
    >,
    nullptr
} };

void GuiModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GuiModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Gui_askConnexionController((*reinterpret_cast< std::add_pointer_t<char*>>(_a[1]))); break;
        case 1: _t->Gui_changeModeController((*reinterpret_cast< std::add_pointer_t<Mode>>(_a[1]))); break;
        case 2: _t->Gui_userAddedController((*reinterpret_cast< std::add_pointer_t<User>>(_a[1]))); break;
        case 3: _t->Gui_userModifiedController((*reinterpret_cast< std::add_pointer_t<User>>(_a[1]))); break;
        case 4: _t->Gui_userRemovedController((*reinterpret_cast< std::add_pointer_t<char*>>(_a[1]))); break;
        case 5: _t->Gui_userSearchedController((*reinterpret_cast< std::add_pointer_t<char*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *GuiModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GuiModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGuiModelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GuiModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
