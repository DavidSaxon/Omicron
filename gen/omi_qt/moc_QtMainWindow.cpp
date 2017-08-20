/****************************************************************************
** Meta object code from reading C++ file 'QtMainWindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/cpp/builtin_subsystems/omi_qt/QtMainWindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtMainWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_omi_qt__QtMainWindow_t {
    QByteArrayData data[4];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_omi_qt__QtMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_omi_qt__QtMainWindow_t qt_meta_stringdata_omi_qt__QtMainWindow = {
    {
QT_MOC_LITERAL(0, 0, 20), // "omi_qt::QtMainWindow"
QT_MOC_LITERAL(1, 21, 10), // "render_now"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 12) // "render_later"

    },
    "omi_qt::QtMainWindow\0render_now\0\0"
    "render_later"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_omi_qt__QtMainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void omi_qt::QtMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtMainWindow *_t = static_cast<QtMainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->render_now(); break;
        case 1: _t->render_later(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject omi_qt::QtMainWindow::staticMetaObject = {
    { &QWindow::staticMetaObject, qt_meta_stringdata_omi_qt__QtMainWindow.data,
      qt_meta_data_omi_qt__QtMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *omi_qt::QtMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *omi_qt::QtMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_omi_qt__QtMainWindow.stringdata0))
        return static_cast<void*>(const_cast< QtMainWindow*>(this));
    if (!strcmp(_clname, "omi::window::MainWindow"))
        return static_cast< omi::window::MainWindow*>(const_cast< QtMainWindow*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(const_cast< QtMainWindow*>(this));
    return QWindow::qt_metacast(_clname);
}

int omi_qt::QtMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
