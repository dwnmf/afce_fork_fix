/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AfcScrollArea_t {
    QByteArrayData data[5];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AfcScrollArea_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AfcScrollArea_t qt_meta_stringdata_AfcScrollArea = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AfcScrollArea"
QT_MOC_LITERAL(1, 14, 9), // "mouseDown"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 11), // "zoomStepped"
QT_MOC_LITERAL(4, 37, 13) // "scrollStepped"

    },
    "AfcScrollArea\0mouseDown\0\0zoomStepped\0"
    "scrollStepped"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AfcScrollArea[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    1,   30,    2, 0x06 /* Public */,
       4,    1,   33,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void AfcScrollArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AfcScrollArea *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mouseDown(); break;
        case 1: _t->zoomStepped((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->scrollStepped((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AfcScrollArea::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AfcScrollArea::mouseDown)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AfcScrollArea::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AfcScrollArea::zoomStepped)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AfcScrollArea::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AfcScrollArea::scrollStepped)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AfcScrollArea::staticMetaObject = { {
    QMetaObject::SuperData::link<QScrollArea::staticMetaObject>(),
    qt_meta_stringdata_AfcScrollArea.data,
    qt_meta_data_AfcScrollArea,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AfcScrollArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AfcScrollArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AfcScrollArea.stringdata0))
        return static_cast<void*>(this);
    return QScrollArea::qt_metacast(_clname);
}

int AfcScrollArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void AfcScrollArea::mouseDown()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void AfcScrollArea::zoomStepped(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AfcScrollArea::scrollStepped(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[55];
    char stringdata0[755];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 11), // "rootChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 14), // "documentLoaded"
QT_MOC_LITERAL(4, 39, 13), // "documentSaved"
QT_MOC_LITERAL(5, 53, 15), // "documentChanged"
QT_MOC_LITERAL(6, 69, 16), // "documentUnloaded"
QT_MOC_LITERAL(7, 86, 13), // "retranslateUi"
QT_MOC_LITERAL(8, 100, 12), // "slotFileOpen"
QT_MOC_LITERAL(9, 113, 11), // "slotFileNew"
QT_MOC_LITERAL(10, 125, 12), // "slotFileSave"
QT_MOC_LITERAL(11, 138, 14), // "slotFileSaveAs"
QT_MOC_LITERAL(12, 153, 14), // "slotFileExport"
QT_MOC_LITERAL(13, 168, 17), // "slotFileExportSVG"
QT_MOC_LITERAL(14, 186, 13), // "slotFilePrint"
QT_MOC_LITERAL(15, 200, 12), // "slotEditCopy"
QT_MOC_LITERAL(16, 213, 11), // "slotEditCut"
QT_MOC_LITERAL(17, 225, 13), // "slotEditPaste"
QT_MOC_LITERAL(18, 239, 14), // "slotEditDelete"
QT_MOC_LITERAL(19, 254, 13), // "slotHelpAbout"
QT_MOC_LITERAL(20, 268, 15), // "slotHelpAboutQt"
QT_MOC_LITERAL(21, 284, 13), // "slotToolArrow"
QT_MOC_LITERAL(22, 298, 15), // "slotToolProcess"
QT_MOC_LITERAL(23, 314, 10), // "slotToolIf"
QT_MOC_LITERAL(24, 325, 11), // "slotToolFor"
QT_MOC_LITERAL(25, 337, 16), // "slotToolWhilePre"
QT_MOC_LITERAL(26, 354, 17), // "slotToolWhilePost"
QT_MOC_LITERAL(27, 372, 10), // "slotToolIo"
QT_MOC_LITERAL(28, 383, 10), // "slotToolOu"
QT_MOC_LITERAL(29, 394, 12), // "slotToolCase"
QT_MOC_LITERAL(30, 407, 17), // "slotToolForCStyle"
QT_MOC_LITERAL(31, 425, 14), // "slotToolAssign"
QT_MOC_LITERAL(32, 440, 17), // "slotDocumentSaved"
QT_MOC_LITERAL(33, 458, 19), // "slotDocumentChanged"
QT_MOC_LITERAL(34, 478, 18), // "slotDocumentLoaded"
QT_MOC_LITERAL(35, 497, 18), // "slotChangeLanguage"
QT_MOC_LITERAL(36, 516, 20), // "slotReloadGenerators"
QT_MOC_LITERAL(37, 537, 7), // "setZoom"
QT_MOC_LITERAL(38, 545, 6), // "quarts"
QT_MOC_LITERAL(39, 552, 9), // "shiftZoom"
QT_MOC_LITERAL(40, 562, 4), // "step"
QT_MOC_LITERAL(41, 567, 12), // "shiftScrollY"
QT_MOC_LITERAL(42, 580, 17), // "slotStatusChanged"
QT_MOC_LITERAL(43, 598, 13), // "slotEditBlock"
QT_MOC_LITERAL(44, 612, 7), // "QBlock*"
QT_MOC_LITERAL(45, 620, 6), // "aBlock"
QT_MOC_LITERAL(46, 627, 13), // "updateActions"
QT_MOC_LITERAL(47, 641, 12), // "generateCode"
QT_MOC_LITERAL(48, 654, 15), // "codeLangChanged"
QT_MOC_LITERAL(49, 670, 5), // "index"
QT_MOC_LITERAL(50, 676, 25), // "docToolsVisibilityChanged"
QT_MOC_LITERAL(51, 702, 7), // "visible"
QT_MOC_LITERAL(52, 710, 24), // "docCodeVisibilityChanged"
QT_MOC_LITERAL(53, 735, 16), // "slotOpenDocument"
QT_MOC_LITERAL(54, 752, 2) // "fn"

    },
    "MainWindow\0rootChanged\0\0documentLoaded\0"
    "documentSaved\0documentChanged\0"
    "documentUnloaded\0retranslateUi\0"
    "slotFileOpen\0slotFileNew\0slotFileSave\0"
    "slotFileSaveAs\0slotFileExport\0"
    "slotFileExportSVG\0slotFilePrint\0"
    "slotEditCopy\0slotEditCut\0slotEditPaste\0"
    "slotEditDelete\0slotHelpAbout\0"
    "slotHelpAboutQt\0slotToolArrow\0"
    "slotToolProcess\0slotToolIf\0slotToolFor\0"
    "slotToolWhilePre\0slotToolWhilePost\0"
    "slotToolIo\0slotToolOu\0slotToolCase\0"
    "slotToolForCStyle\0slotToolAssign\0"
    "slotDocumentSaved\0slotDocumentChanged\0"
    "slotDocumentLoaded\0slotChangeLanguage\0"
    "slotReloadGenerators\0setZoom\0quarts\0"
    "shiftZoom\0step\0shiftScrollY\0"
    "slotStatusChanged\0slotEditBlock\0QBlock*\0"
    "aBlock\0updateActions\0generateCode\0"
    "codeLangChanged\0index\0docToolsVisibilityChanged\0"
    "visible\0docCodeVisibilityChanged\0"
    "slotOpenDocument\0fn"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      46,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  244,    2, 0x06 /* Public */,
       3,    0,  245,    2, 0x06 /* Public */,
       4,    0,  246,    2, 0x06 /* Public */,
       5,    0,  247,    2, 0x06 /* Public */,
       6,    0,  248,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  249,    2, 0x0a /* Public */,
       8,    0,  250,    2, 0x0a /* Public */,
       9,    0,  251,    2, 0x0a /* Public */,
      10,    0,  252,    2, 0x0a /* Public */,
      11,    0,  253,    2, 0x0a /* Public */,
      12,    0,  254,    2, 0x0a /* Public */,
      13,    0,  255,    2, 0x0a /* Public */,
      14,    0,  256,    2, 0x0a /* Public */,
      15,    0,  257,    2, 0x0a /* Public */,
      16,    0,  258,    2, 0x0a /* Public */,
      17,    0,  259,    2, 0x0a /* Public */,
      18,    0,  260,    2, 0x0a /* Public */,
      19,    0,  261,    2, 0x0a /* Public */,
      20,    0,  262,    2, 0x0a /* Public */,
      21,    0,  263,    2, 0x0a /* Public */,
      22,    0,  264,    2, 0x0a /* Public */,
      23,    0,  265,    2, 0x0a /* Public */,
      24,    0,  266,    2, 0x0a /* Public */,
      25,    0,  267,    2, 0x0a /* Public */,
      26,    0,  268,    2, 0x0a /* Public */,
      27,    0,  269,    2, 0x0a /* Public */,
      28,    0,  270,    2, 0x0a /* Public */,
      29,    0,  271,    2, 0x0a /* Public */,
      30,    0,  272,    2, 0x0a /* Public */,
      31,    0,  273,    2, 0x0a /* Public */,
      32,    0,  274,    2, 0x0a /* Public */,
      33,    0,  275,    2, 0x0a /* Public */,
      34,    0,  276,    2, 0x0a /* Public */,
      35,    0,  277,    2, 0x0a /* Public */,
      36,    0,  278,    2, 0x0a /* Public */,
      37,    1,  279,    2, 0x0a /* Public */,
      39,    1,  282,    2, 0x0a /* Public */,
      41,    1,  285,    2, 0x0a /* Public */,
      42,    0,  288,    2, 0x0a /* Public */,
      43,    1,  289,    2, 0x0a /* Public */,
      46,    0,  292,    2, 0x0a /* Public */,
      47,    0,  293,    2, 0x0a /* Public */,
      48,    1,  294,    2, 0x0a /* Public */,
      50,    1,  297,    2, 0x0a /* Public */,
      52,    1,  300,    2, 0x0a /* Public */,
      53,    1,  303,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   38,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void, QMetaType::Int,   40,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 44,   45,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   49,
    QMetaType::Void, QMetaType::Bool,   51,
    QMetaType::Void, QMetaType::Bool,   51,
    QMetaType::Void, QMetaType::QString,   54,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->rootChanged(); break;
        case 1: _t->documentLoaded(); break;
        case 2: _t->documentSaved(); break;
        case 3: _t->documentChanged(); break;
        case 4: _t->documentUnloaded(); break;
        case 5: _t->retranslateUi(); break;
        case 6: _t->slotFileOpen(); break;
        case 7: _t->slotFileNew(); break;
        case 8: _t->slotFileSave(); break;
        case 9: _t->slotFileSaveAs(); break;
        case 10: _t->slotFileExport(); break;
        case 11: _t->slotFileExportSVG(); break;
        case 12: _t->slotFilePrint(); break;
        case 13: _t->slotEditCopy(); break;
        case 14: _t->slotEditCut(); break;
        case 15: _t->slotEditPaste(); break;
        case 16: _t->slotEditDelete(); break;
        case 17: _t->slotHelpAbout(); break;
        case 18: _t->slotHelpAboutQt(); break;
        case 19: _t->slotToolArrow(); break;
        case 20: _t->slotToolProcess(); break;
        case 21: _t->slotToolIf(); break;
        case 22: _t->slotToolFor(); break;
        case 23: _t->slotToolWhilePre(); break;
        case 24: _t->slotToolWhilePost(); break;
        case 25: _t->slotToolIo(); break;
        case 26: _t->slotToolOu(); break;
        case 27: _t->slotToolCase(); break;
        case 28: _t->slotToolForCStyle(); break;
        case 29: _t->slotToolAssign(); break;
        case 30: _t->slotDocumentSaved(); break;
        case 31: _t->slotDocumentChanged(); break;
        case 32: _t->slotDocumentLoaded(); break;
        case 33: _t->slotChangeLanguage(); break;
        case 34: _t->slotReloadGenerators(); break;
        case 35: _t->setZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->shiftZoom((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->shiftScrollY((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->slotStatusChanged(); break;
        case 39: _t->slotEditBlock((*reinterpret_cast< QBlock*(*)>(_a[1]))); break;
        case 40: _t->updateActions(); break;
        case 41: _t->generateCode(); break;
        case 42: _t->codeLangChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->docToolsVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->docCodeVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 45: _t->slotOpenDocument((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 39:
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
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::rootChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::documentLoaded)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::documentSaved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::documentChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::documentUnloaded)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 46)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 46;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 46)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 46;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::rootChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::documentLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::documentSaved()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::documentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MainWindow::documentUnloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
