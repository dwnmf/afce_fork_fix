/****************************************************************************
** Meta object code from reading C++ file 'zvflowchart.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../zvflowchart.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'zvflowchart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QBlock_t {
    QByteArrayData data[1];
    char stringdata0[7];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QBlock_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QBlock_t qt_meta_stringdata_QBlock = {
    {
QT_MOC_LITERAL(0, 0, 6) // "QBlock"

    },
    "QBlock"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QBlock[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QBlock::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject QBlock::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_QBlock.data,
    qt_meta_data_QBlock,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QBlock::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QBlock::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QBlock.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QBlock::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_QFlowChart_t {
    QByteArrayData data[24];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QFlowChart_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QFlowChart_t qt_meta_stringdata_QFlowChart = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QFlowChart"
QT_MOC_LITERAL(1, 11, 11), // "zoomChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 5), // "aZoom"
QT_MOC_LITERAL(4, 30, 13), // "statusChanged"
QT_MOC_LITERAL(5, 44, 9), // "editBlock"
QT_MOC_LITERAL(6, 54, 7), // "QBlock*"
QT_MOC_LITERAL(7, 62, 5), // "block"
QT_MOC_LITERAL(8, 68, 7), // "changed"
QT_MOC_LITERAL(9, 76, 8), // "modified"
QT_MOC_LITERAL(10, 85, 5), // "clear"
QT_MOC_LITERAL(11, 91, 9), // "selectAll"
QT_MOC_LITERAL(12, 101, 11), // "deselectAll"
QT_MOC_LITERAL(13, 113, 7), // "setZoom"
QT_MOC_LITERAL(14, 121, 9), // "setStatus"
QT_MOC_LITERAL(15, 131, 7), // "aStatus"
QT_MOC_LITERAL(16, 139, 17), // "deleteActiveBlock"
QT_MOC_LITERAL(17, 157, 14), // "realignObjects"
QT_MOC_LITERAL(18, 172, 9), // "setBuffer"
QT_MOC_LITERAL(19, 182, 7), // "aBuffer"
QT_MOC_LITERAL(20, 190, 14), // "setMultiInsert"
QT_MOC_LITERAL(21, 205, 6), // "aValue"
QT_MOC_LITERAL(22, 212, 4), // "undo"
QT_MOC_LITERAL(23, 217, 4) // "redo"

    },
    "QFlowChart\0zoomChanged\0\0aZoom\0"
    "statusChanged\0editBlock\0QBlock*\0block\0"
    "changed\0modified\0clear\0selectAll\0"
    "deselectAll\0setZoom\0setStatus\0aStatus\0"
    "deleteActiveBlock\0realignObjects\0"
    "setBuffer\0aBuffer\0setMultiInsert\0"
    "aValue\0undo\0redo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QFlowChart[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       4,    0,   97,    2, 0x06 /* Public */,
       5,    1,   98,    2, 0x06 /* Public */,
       8,    0,  101,    2, 0x06 /* Public */,
       9,    0,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  103,    2, 0x0a /* Public */,
      11,    0,  104,    2, 0x0a /* Public */,
      12,    0,  105,    2, 0x0a /* Public */,
      13,    1,  106,    2, 0x0a /* Public */,
      14,    1,  109,    2, 0x0a /* Public */,
      16,    0,  112,    2, 0x0a /* Public */,
      17,    0,  113,    2, 0x0a /* Public */,
      18,    1,  114,    2, 0x0a /* Public */,
      20,    1,  117,    2, 0x0a /* Public */,
      22,    0,  120,    2, 0x0a /* Public */,
      23,    0,  121,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::Bool,   21,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QFlowChart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QFlowChart *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomChanged((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 1: _t->statusChanged(); break;
        case 2: _t->editBlock((*reinterpret_cast< QBlock*(*)>(_a[1]))); break;
        case 3: _t->changed(); break;
        case 4: _t->modified(); break;
        case 5: _t->clear(); break;
        case 6: _t->selectAll(); break;
        case 7: _t->deselectAll(); break;
        case 8: _t->setZoom((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 9: _t->setStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->deleteActiveBlock(); break;
        case 11: _t->realignObjects(); break;
        case 12: _t->setBuffer((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->setMultiInsert((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->undo(); break;
        case 15: _t->redo(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QBlock* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QFlowChart::*)(const double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFlowChart::zoomChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QFlowChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFlowChart::statusChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QFlowChart::*)(QBlock * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFlowChart::editBlock)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (QFlowChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFlowChart::changed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (QFlowChart::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QFlowChart::modified)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QFlowChart::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_QFlowChart.data,
    qt_meta_data_QFlowChart,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QFlowChart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QFlowChart::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QFlowChart.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QFlowChart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void QFlowChart::zoomChanged(const double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QFlowChart::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QFlowChart::editBlock(QBlock * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void QFlowChart::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QFlowChart::modified()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
