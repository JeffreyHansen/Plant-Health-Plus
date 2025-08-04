# Plant Health + (Plus)

A desktop plant-tracking software that provides the ability to track, monitor, and log plants.

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [How to Use](#how-to-use)
3. [Troubleshooting](#troubleshooting)
3. [Contributors](#contributors)


## Prerequisites

- Qt 6.x (install via Qt installer for Windows, or Homebrew for macOS)
- CMake 3.16 or higher
- A compatible C++ compiler (MSVC, MinGW for Windows; Xcode Command Line Tools for macOS)
- Git (optional, for cloning the repository)

## How to Use

### Quick Build (Recommended)

For convenience, use the automated build scripts:

**On macOS/Linux:**
```bash
./build.sh
```

**On Windows:**
```batch
build.bat
```

These scripts will automatically detect your operating system, check prerequisites, and build the project with proper deployment.

### Manual Build Instructions

### Windows:
1. Clone the repository:
```bash
git clone https://github.com/JeffreyHansen/Plant-Health-Plus
cd Plant-Health-Plus/PlantHealthPlus
```

2. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```

3. Configure the project with CMake:
```bash
cmake ..
```

4. Build the project (Release mode):
```bash
cmake --build . --config Release
```

5. Navigate to the Release folder and run windeployqt to bundle Qt dependencies:
```bash
cd Release
"C:/Qt/6.9.1/mingw_64/bin/windeployqt.exe" PlantHealthPlus.exe
```

The executable `PlantHealthPlus.exe` will be ready to run in the Release folder.

### macOS:
1. Clone the repository:
```bash
git clone https://github.com/JeffreyHansen/Plant-Health-Plus
cd Plant-Health-Plus/PlantHealthPlus
```

2. Install Qt6 via Homebrew (if not already installed):
```bash
brew install qt@6
```

3. Create a build directory and navigate to it:
```bash
mkdir build
cd build
```

4. Configure the project with CMake:
```bash
cmake ..
```

5. Build the project:
```bash
cmake --build . --config Release
```

6. Deploy Qt dependencies and create the app bundle:
```bash
/opt/homebrew/opt/qt@6/bin/macdeployqt PlantHealthPlus.app
```

The application bundle `PlantHealthPlus.app` will be ready to run. You can either:
- Double-click the app bundle to launch it
- Run it from terminal: `open PlantHealthPlus.app`

## Troubleshooting

### Windows:
- **Missing DLLs?** Run windeployqt to bundle Qt dependencies.
- **Permission issues?** Run Command Prompt as Administrator.
- **CMake can't find Qt?** Ensure Qt is installed and CMAKE_PREFIX_PATH is set correctly.
- **Build fails?** Check that you have the correct Qt version (6.x) and a compatible compiler.

### macOS:
- **CMake can't find Qt?** Install Qt via Homebrew: `brew install qt@6`
- **Permission issues?** Ensure Xcode Command Line Tools are installed: `xcode-select --install`
- **App won't launch due to code signing?** The build script automatically signs the app with an ad-hoc signature. If you still have issues, manually run: `codesign --force --deep --sign - PlantHealthPlus.app`
- **App icon not showing?** The build script automatically copies the icon file to the app bundle.
- **Build fails on Apple Silicon?** Ensure you're using the correct Qt build for your architecture.
- **"App is damaged" error?** Remove extended attributes: `xattr -cr PlantHealthPlus.app` then re-sign the app.

## Contributors
Jeffrey Hansen, Tal Faitelson, Daniel Hernandez Vigil, Ashley Flores

