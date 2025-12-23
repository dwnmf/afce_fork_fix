@echo off
setlocal

:: --- НАСТРОЙКИ ПУТЕЙ ---
:: Укажите здесь пути к вашему Qt и MinGW
set QT_BIN_PATH=D:\Programs\Qt\5.15.2\mingw81_64\bin
:: Если mingw32-make уже в PATH, вторую строку можно не менять
set MINGW_BIN_PATH=D:\Programs\Qt\Tools\mingw810_64\bin

:: Добавляем их в PATH для текущего окна
set PATH=%QT_BIN_PATH%;%MINGW_BIN_PATH%;%PATH%

echo [1/4] Preparing build directory...
if exist build (
    echo Cleaning old build...
    rmdir /s /q build
)
mkdir build
cd build

echo [2/4] Running qmake...
qmake ..\afce.pro
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] qmake failed!
    pause
    exit /b %errorlevel%
)

echo [3/4] Running mingw32-make...
mingw32-make -f Makefile.Release
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b %errorlevel%
)

echo [4/4] Done! 
echo.
echo Building executable is located in: build\release\afce.exe
echo.
pause
