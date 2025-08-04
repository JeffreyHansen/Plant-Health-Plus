@echo off
setlocal enabledelayedexpansion

REM Plant Health Plus - Windows Build Script
REM Automatically builds and deploys the Plant Health Plus application

echo 🌱 Plant Health Plus - Windows Build Script
echo ==========================================

REM Set color codes (if supported)
set "INFO=[INFO]"
set "SUCCESS=[SUCCESS]"
set "WARNING=[WARNING]"
set "ERROR=[ERROR]"

REM Check if we're on Windows
if not "%OS%"=="Windows_NT" (
    echo %ERROR% This script is for Windows only. Use build.sh for macOS/Linux.
    exit /b 1
)

REM Navigate to the project directory
set "SCRIPT_DIR=%~dp0"
set "PROJECT_DIR=%SCRIPT_DIR%PlantHealthPlus"

if not exist "%PROJECT_DIR%" (
    echo %ERROR% PlantHealthPlus directory not found. Make sure you're running this from the project root.
    exit /b 1
)

cd /d "%PROJECT_DIR%"

echo %INFO% Checking prerequisites...

REM Check for CMake
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo %ERROR% CMake not found. Please install CMake from https://cmake.org/download/
    exit /b 1
)

for /f "tokens=3" %%i in ('cmake --version 2^>nul ^| findstr "cmake version"') do set CMAKE_VERSION=%%i
echo %INFO% Found CMake version: %CMAKE_VERSION%

REM Try to find Qt6 installation
set "QT_PATH="
set "QT_DEPLOYQT="

REM Common Qt installation paths
for %%p in (
    "C:\Qt\6.8.1\mingw_64"
    "C:\Qt\6.8.0\mingw_64"
    "C:\Qt\6.7.2\mingw_64"
    "C:\Qt\6.7.1\mingw_64"
    "C:\Qt\6.7.0\mingw_64"
    "C:\Qt\6.6.3\mingw_64"
    "C:\Qt\6.6.2\mingw_64"
    "C:\Qt\6.6.1\mingw_64"
    "C:\Qt\6.6.0\mingw_64"
    "C:\Qt\6.5.3\mingw_64"
    "C:\Qt\6.5.2\mingw_64"
    "C:\Qt\6.5.1\mingw_64"
    "C:\Qt\6.5.0\mingw_64"
    "C:\Qt\6.4.3\mingw_64"
    "C:\Qt\6.4.2\mingw_64"
    "C:\Qt\6.4.1\mingw_64"
    "C:\Qt\6.4.0\mingw_64"
    "C:\Qt\Tools\Qt Creator\bin\qt6"
) do (
    if exist "%%~p\bin\qmake.exe" (
        set "QT_PATH=%%~p"
        set "QT_DEPLOYQT=%%~p\bin\windeployqt.exe"
        goto :qt_found
    )
)

:qt_found
if "%QT_PATH%"=="" (
    echo %ERROR% Qt6 not found in common installation paths.
    echo %ERROR% Please install Qt6 from https://www.qt.io/download-qt-installer
    echo %ERROR% Or set QT_PATH environment variable to your Qt installation.
    exit /b 1
)

echo %INFO% Found Qt6 at: %QT_PATH%

REM Check for compiler (try to find Visual Studio or MinGW)
set "COMPILER_FOUND=0"

REM Check for Visual Studio
where cl >nul 2>&1
if %errorlevel% equ 0 (
    set "COMPILER_FOUND=1"
    echo %INFO% Found Visual Studio compiler
)

REM Check for MinGW (usually comes with Qt)
where gcc >nul 2>&1
if %errorlevel% equ 0 (
    set "COMPILER_FOUND=1"
    echo %INFO% Found MinGW compiler
)

REM Check for MinGW in Qt directory
if exist "%QT_PATH%\bin\gcc.exe" (
    set "COMPILER_FOUND=1"
    echo %INFO% Found MinGW compiler in Qt installation
    set "PATH=%QT_PATH%\bin;%PATH%"
)

if %COMPILER_FOUND% equ 0 (
    echo %ERROR% No compatible compiler found.
    echo %ERROR% Please install Visual Studio 2019+ or ensure MinGW is in PATH.
    exit /b 1
)

echo %INFO% Starting build process...

REM Clean previous build
if exist "build" (
    echo %INFO% Cleaning previous build...
    rmdir /s /q build
)

REM Create build directory
echo %INFO% Creating build directory...
mkdir build
cd build

REM Configure project with CMake
echo %INFO% Configuring project with CMake...
cmake -DCMAKE_PREFIX_PATH="%QT_PATH%\lib\cmake" -DCMAKE_BUILD_TYPE=Release ..

if %errorlevel% neq 0 (
    echo %ERROR% CMake configuration failed
    exit /b 1
)

REM Build the project
echo %INFO% Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo %ERROR% Build failed
    exit /b 1
)

echo %SUCCESS% Build completed successfully!

REM Navigate to Release directory
if exist "Release" (
    cd Release
) else (
    echo %WARNING% Release directory not found, looking for executable in current directory...
)

REM Find the executable
set "EXE_PATH="
if exist "PlantHealthPlus.exe" (
    set "EXE_PATH=PlantHealthPlus.exe"
) else if exist "..\Release\PlantHealthPlus.exe" (
    cd ..\Release
    set "EXE_PATH=PlantHealthPlus.exe"
) else (
    echo %ERROR% PlantHealthPlus.exe not found
    exit /b 1
)

echo %INFO% Found executable: %EXE_PATH%

REM Deploy Qt dependencies
echo %INFO% Deploying Qt dependencies...
if not exist "%QT_DEPLOYQT%" (
    echo %ERROR% windeployqt not found at %QT_DEPLOYQT%
    exit /b 1
)

echo %INFO% Running windeployqt...
"%QT_DEPLOYQT%" "%EXE_PATH%" --release --compiler-runtime

if %errorlevel% neq 0 (
    echo %WARNING% windeployqt completed with warnings (this is usually normal)
else
    echo %SUCCESS% Qt dependencies deployed successfully
)

REM Copy any additional resources if needed
if exist "..\..\images\PHPlus_logo.png" (
    if not exist "PHPlus_logo.png" (
        echo %INFO% Copying application icon...
        copy "..\..\images\PHPlus_logo.png" .
        echo %SUCCESS% Application icon copied
    )
)

REM Final checks and information
if exist "%EXE_PATH%" (
    echo %SUCCESS% Windows application created successfully!
    echo %INFO% Executable location: %CD%\%EXE_PATH%
    
    REM Get directory size
    for /f "tokens=3" %%a in ('dir /-c ^| find "File(s)"') do set "APP_SIZE=%%a"
    echo %INFO% Application directory size: %APP_SIZE% bytes
    
    echo.
    echo %SUCCESS% 🚀 Build completed successfully!
    echo.
    echo To run the application:
    echo   %EXE_PATH%
    echo.
    echo To distribute:
    echo   Copy the entire Release folder ^(contains all dependencies^)
    echo.
    echo To create installer:
    echo   Use NSIS, Inno Setup, or similar installer creation tool
else
    echo %ERROR% Executable not found. Build may have failed.
    exit /b 1
)

pause
