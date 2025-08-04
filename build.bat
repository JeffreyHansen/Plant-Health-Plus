@echo off
REM Plant Health Plus Build Script for Windows
REM Automatically builds the project and deploys dependencies

setlocal enabledelayedexpansion

echo [INFO] Plant Health Plus Build Script
echo [INFO] ==============================

REM Check if we're in the right directory
if not exist "PlantHealthPlus" (
    echo [ERROR] PlantHealthPlus directory not found. Are you in the correct directory?
    pause
    exit /b 1
)

REM Check for CMake
cmake --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] CMake is not installed or not in PATH. Please install CMake 3.16 or higher.
    pause
    exit /b 1
)

echo [INFO] CMake found

REM Navigate to PlantHealthPlus directory
cd PlantHealthPlus

REM Create build directory
echo [INFO] Creating build directory...
if not exist "build" mkdir build
cd build

REM Configure with CMake
echo [INFO] Configuring project with CMake...
cmake ..
if errorlevel 1 (
    echo [ERROR] CMake configuration failed
    pause
    exit /b 1
)

REM Build the project
echo [INFO] Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

REM Deploy Windows application
echo [INFO] Deploying Windows application...
if exist "Release\PlantHealthPlus.exe" (
    cd Release
    
    REM Try to find windeployqt in common Qt installation paths
    set "WINDEPLOYQT_FOUND=false"
    
    REM Check various Qt versions and paths
    for %%v in (6.9.1 6.8.0 6.7.0 6.6.0) do (
        if exist "C:\Qt\%%v\mingw_64\bin\windeployqt.exe" (
            echo [INFO] Found windeployqt at C:\Qt\%%v\mingw_64\bin\windeployqt.exe
            "C:\Qt\%%v\mingw_64\bin\windeployqt.exe" PlantHealthPlus.exe
            set "WINDEPLOYQT_FOUND=true"
            goto :deploy_done
        )
    )
    
    REM Try MSVC versions
    for %%v in (6.9.1 6.8.0 6.7.0 6.6.0) do (
        if exist "C:\Qt\%%v\msvc2022_64\bin\windeployqt.exe" (
            echo [INFO] Found windeployqt at C:\Qt\%%v\msvc2022_64\bin\windeployqt.exe
            "C:\Qt\%%v\msvc2022_64\bin\windeployqt.exe" PlantHealthPlus.exe
            set "WINDEPLOYQT_FOUND=true"
            goto :deploy_done
        )
    )
    
    :deploy_done
    if "!WINDEPLOYQT_FOUND!"=="true" (
        echo [SUCCESS] Windows executable created and deployed successfully!
        echo [INFO] You can run the app with: PlantHealthPlus.exe
    ) else (
        echo [WARNING] windeployqt not found in common Qt installation paths.
        echo [WARNING] Executable may be missing dependencies.
        echo [INFO] Please run windeployqt manually from your Qt installation.
        echo [INFO] Example: "C:\Qt\6.9.1\mingw_64\bin\windeployqt.exe" PlantHealthPlus.exe
    )
    
    cd ..
) else (
    echo [ERROR] PlantHealthPlus.exe not found after build
    pause
    exit /b 1
)

echo [SUCCESS] Build completed successfully!
echo [INFO] Check the build\Release directory for your executable.
pause
