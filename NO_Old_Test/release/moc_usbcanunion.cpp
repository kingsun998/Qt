/****************************************************************************
** Meta object code from reading C++ file 'usbcanunion.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../usbcanunion.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usbcanunion.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UsbCanUnion_t {
    QByteArrayData data[10];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UsbCanUnion_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UsbCanUnion_t qt_meta_stringdata_UsbCanUnion = {
    {
QT_MOC_LITERAL(0, 0, 11), // "UsbCanUnion"
QT_MOC_LITERAL(1, 12, 17), // "setFrameStartTime"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 14), // "openInitUsbCan"
QT_MOC_LITERAL(4, 46, 11), // "closeUsbCan"
QT_MOC_LITERAL(5, 58, 11), // "startUsbCan"
QT_MOC_LITERAL(6, 70, 9), // "channelid"
QT_MOC_LITERAL(7, 80, 14), // "ReceiveFromCan"
QT_MOC_LITERAL(8, 95, 9), // "SendToCan"
QT_MOC_LITERAL(9, 105, 13) // "StartTestMode"

    },
    "UsbCanUnion\0setFrameStartTime\0\0"
    "openInitUsbCan\0closeUsbCan\0startUsbCan\0"
    "channelid\0ReceiveFromCan\0SendToCan\0"
    "StartTestMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UsbCanUnion[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    1,   52,    2, 0x0a /* Public */,
       7,    1,   55,    2, 0x0a /* Public */,
       8,    1,   58,    2, 0x0a /* Public */,
       9,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, QMetaType::UInt,    6,
    QMetaType::UInt, QMetaType::UInt,    6,
    QMetaType::Int, QMetaType::UInt,    6,
    QMetaType::Int,

       0        // eod
};

void UsbCanUnion::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UsbCanUnion *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setFrameStartTime(); break;
        case 1: { int _r = _t->openInitUsbCan();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->closeUsbCan();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: { int _r = _t->startUsbCan((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { uint _r = _t->ReceiveFromCan((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< uint*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->SendToCan((*reinterpret_cast< uint(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->StartTestMode();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UsbCanUnion::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UsbCanUnion::setFrameStartTime)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UsbCanUnion::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_UsbCanUnion.data,
    qt_meta_data_UsbCanUnion,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UsbCanUnion::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UsbCanUnion::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UsbCanUnion.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UsbCanUnion::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void UsbCanUnion::setFrameStartTime()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
