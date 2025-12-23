# AFCE — Algorithm Flowchart Editor

## Обзор проекта

**AFCE** (Algorithm Flowchart Editor) — редактор блок-схем алгоритмов, написанный на C++ с использованием Qt 5. Позволяет создавать, редактировать, экспортировать и печатать блок-схемы в соответствии с ГОСТ 19.701-90.

**Версия**: 0.9.9-alpha  
**Лицензия**: GPL v2/v3  
**Автор**: Viktor Zinkevich

---

## Структура проекта

```
afce-master/
├── main.cpp                 # Точка входа приложения
├── mainwindow.cpp/.h        # Главное окно приложения (MainWindow)
├── zvflowchart.cpp/.h       # Ядро: QFlowChart (виджет) и QBlock (блоки)
├── qflowchartstyle.cpp/.h   # Стили оформления блок-схем
├── sourcecodegenerator.cpp/.h # Генерация кода из блок-схем
├── thelpwindow.cpp/.h       # Окно справки
├── afce.pro                 # Файл проекта Qt
├── afce.qrc                 # Ресурсы (иконки, изображения)
├── generators/              # JSON-шаблоны генерации кода (C, Pascal, Python и др.)
├── locale/                  # Переводы (ru_RU, uk_UA, en_US)
├── help/                    # HTML-справка
├── build/                   # Директория сборки
└── info.md                  # План текущих работ
```

---
d
## Архитектура

### Главные классы

#### 1. `QFlowChart` (zvflowchart.h/cpp)
**Виджет Qt**, отображающий блок-схему и обрабатывающий взаимодействие пользователя.

**Ключевые методы:**
- `paintEvent()` — отрисовка схемы
- `mousePressEvent()` / `mouseDoubleClickEvent()` — обработка кликов
- `realignObjects()` — пересчёт размеров и позиций всех блоков
- `regeneratePoints()` — обновление точек вставки
- `setZoom()` — масштабирование
- `undo()` / `redo()` — отмена/повтор действий

**Состояния (`status()`):**
- `Display` — только просмотр
- `Selectable` — выбор блоков (режим по умолчанию)
- `Insertion` — вставка нового блока

#### 2. `QBlock` (zvflowchart.h/cpp)
**Узел блок-схемы** — может быть блоком (process, if, for и т.д.) или веткой (branch).

**Ключевые поля:**
```cpp
QHash<QString, QString> attributes;  // Атрибуты блока (text, cond, vars и др.)
double x, y, width, height;          // Геометрия
QList<QBlock *> items;               // Дочерние элементы
QBlock *parent;                      // Родитель
bool isBranch;                       // Является ли веткой
```

**Ключевые методы:**
- `adjustSize(zoom)` — вычисляет размеры блока с учётом содержимого
- `adjustPosition(ox, oy)` — позиционирует блок и дочерние элементы
- `paint(canvas)` — отрисовка блока
- `xmlNode()` / `setXmlNode()` — сериализация в XML

**Типы блоков (`type()`):**
| Тип | Описание | Атрибуты |
|-----|----------|----------|
| `algorithm` | Корневой блок (BEGIN/END) | — |
| `process` | Процесс (прямоугольник) | `text` |
| `assign` | Присваивание | `dest`, `src` |
| `if` | Ветвление (ромб) | `cond` |
| `for` | Цикл FOR (шестиугольник) | `var`, `from`, `to` |
| `pre` | Цикл WHILE с предусловием | `cond` |
| `post` | Цикл с постусловием | `cond` |
| `io` | Ввод (параллелограмм) | `vars` (через запятую) |
| `ou` | Вывод (параллелограмм) | `vars` (через запятую) |

#### 3. `MainWindow` (mainwindow.h/cpp)
**Главное окно приложения** с меню, панелью инструментов и областью редактирования.

**Ключевые слоты:**
- `slotEditBlock(QBlock*)` — редактирование блока через диалог
- `slotFileSave()` / `slotFileOpen()` — сохранение/загрузка .afc
- `slotFileExport()` / `slotFileExportSVG()` — экспорт в PNG/SVG
- `slotFilePrint()` — печать
- `slotTool*()` — вставка блоков разных типов

#### 4. `SourceCodeGenerator` (sourcecodegenerator.h/cpp)
Генерирует исходный код из блок-схемы по JSON-шаблонам из `generators/`.

#### 5. `QFlowChartStyle` (qflowchartstyle.h/cpp)
Хранит цвета и стили линий для отрисовки схемы.

---

## Формат файла .afc

Файлы сохраняются в формате **XML**:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<algorithm version="1.2">
  <branch>
    <io vars="x,y"/>
    <assign dest="sum" src="x + y"/>
    <ou vars="sum"/>
  </branch>
</algorithm>
```

---

## ГОСТ 19.701-90

Размеры блоков кратны модулю `a = 60px` (при zoom=1):
- Высота блока: `a = 60px`
- Ширина блока: `b = 2a = 120px` (минимум, расширяется под текст)
- Шрифт: Tahoma, 13px

---

## Ключевые алгоритмы

### Пересчёт размеров (`realignObjects`)
```
1. root->adjustSize(zoom)     // Рекурсивно вычисляет width/height
2. root->adjustPosition(0,0)  // Рекурсивно позиционирует блоки
3. regeneratePoints()         // Обновляет точки вставки
4. resize(sizeHint())         // Обновляет размер виджета
5. update()                   // Перерисовка
```

### Динамическое расширение блоков
В `QBlock::adjustSize()` для блоков `process`, `assign`, `io`, `ou`:
```cpp
QFont font("Tahoma");
font.setPixelSize(13 * aZoom);
QFontMetrics fm(font);
double textWidth = fm.horizontalAdvance(text) + padding;
clientWidth = qMax(minWidth, textWidth);
```

### Редактирование блока
После изменения атрибутов в `slotEditBlock()`:
```cpp
aBlock->flowChart()->makeUndo();
aBlock->attributes[...] = ...;
aBlock->flowChart()->realignObjects();  // ← Немедленный пересчёт
aBlock->flowChart()->update();
aBlock->flowChart()->makeChanged();
```

---

## Сборка

### Требования
- Qt 5.x (тестировалось на 5.15.2)
- MinGW или MSVC

### Команды
```bash
cd afce-master
mkdir build && cd build
qmake ../afce.pro
mingw32-make        # или make на Linux
```

Исполняемый файл: `build/release/afce.exe`

---

## Недавние изменения

### Расширяемые блоки (декабрь 2024)
- Блоки `process`, `assign`, `io`, `ou` автоматически расширяются под длину текста
- Добавлен вызов `realignObjects()` после редактирования для немедленного обновления
- Диалоги редактирования стали растягиваемыми (size grip, stretch в layout'ах)

---

## TODO

- [ ] Проверить сохранение/загрузку после изменений
- [ ] Проверить экспорт PNG/SVG с длинным текстом
- [ ] Проверить печать
- [ ] Добавить расширение для блоков `if`, `for`, `pre`, `post`
