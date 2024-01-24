/****************************************************************************
** Meta object code from reading C++ file 'GuiController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Visiolock/src/GuiController.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GuiController.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSGuiControllerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSGuiControllerENDCLASS = QtMocHelpers::stringData(
    "GuiController",
    "screenStatusChanged",
    "",
    "passResultChanged",
    "newUserTagChanged",
    "displayHomeScreenChanged",
    "screenStatus",
    "passResult",
    "newUserTag",
    "authResult"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSGuiControllerENDCLASS_t {
    uint offsetsAndSizes[20];
    char stringdata0[14];
    char stringdata1[20];
    char stringdata2[1];
    char stringdata3[18];
    char stringdata4[18];
    char stringdata5[25];
    char stringdata6[13];
    char stringdata7[11];
    char stringdata8[11];
    char stringdata9[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSGuiControllerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSGuiControllerENDCLASS_t qt_meta_stringdata_CLASSGuiControllerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 13),  // "GuiController"
        QT_MOC_LITERAL(14, 19),  // "screenStatusChanged"
        QT_MOC_LITERAL(34, 0),  // ""
        QT_MOC_LITERAL(35, 17),  // "passResultChanged"
        QT_MOC_LITERAL(53, 17),  // "newUserTagChanged"
        QT_MOC_LITERAL(71, 24),  // "displayHomeScreenChanged"
        QT_MOC_LITERAL(96, 12),  // "screenStatus"
        QT_MOC_LITERAL(109, 10),  // "passResult"
        QT_MOC_LITERAL(120, 10),  // "newUserTag"
        QT_MOC_LITERAL(131, 10)   // "authResult"
    },
    "GuiController",
    "screenStatusChanged",
    "",
    "passResultChanged",
    "newUserTagChanged",
    "displayHomeScreenChanged",
    "screenStatus",
    "passResult",
    "newUserTag",
    "authResult"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGuiControllerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       4,   42, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   38,    2, 0x06,    5 /* Public */,
       3,    0,   39,    2, 0x06,    6 /* Public */,
       4,    0,   40,    2, 0x06,    7 /* Public */,
       5,    0,   41,    2, 0x06,    8 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
       6, QMetaType::Bool, 0x00015003, uint(0), 0,
       7, QMetaType::Bool, 0x00015003, uint(1), 0,
       8, QMetaType::QString, 0x00015003, uint(2), 0,
       9, QMetaType::Int, 0x00015003, uint(3), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject GuiController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSGuiControllerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGuiControllerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSGuiControllerENDCLASS_t,
        // property 'screenStatus'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'passResult'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'newUserTag'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'authResult'
        QtPrivate::TypeAndForceComplete<int, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GuiController, std::true_type>,
        // method 'screenStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'passResultChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'newUserTagChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'displayHomeScreenChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GuiController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GuiController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->screenStatusChanged(); break;
        case 1: _t->passResultChanged(); break;
        case 2: _t->newUserTagChanged(); break;
        case 3: _t->displayHomeScreenChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GuiController::*)();
            if (_t _q_method = &GuiController::screenStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GuiController::*)();
            if (_t _q_method = &GuiController::passResultChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GuiController::*)();
            if (_t _q_method = &GuiController::newUserTagChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GuiController::*)();
            if (_t _q_method = &GuiController::displayHomeScreenChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<GuiController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->getScreenStatus(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->getPassResult(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->getNewUserTag(); break;
        case 3: *reinterpret_cast< int*>(_v) = _t->getAuthResult(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<GuiController *>(_o);
        (void)_t;
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->GuiController_activateScreen(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->GuiController_passResult(*reinterpret_cast< bool*>(_v)); break;
        case 2: _t->GuiController_displayNewUserTag(*reinterpret_cast< QString*>(_v)); break;
        case 3: _t->Gui_Controller_displayHomeScreen(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    } else if (_c == QMetaObject::BindableProperty) {
    }
    (void)_a;
}

const QMetaObject *GuiController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GuiController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGuiControllerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GuiController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void GuiController::screenStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GuiController::passResultChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void GuiController::newUserTagChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void GuiController::displayHomeScreenChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
