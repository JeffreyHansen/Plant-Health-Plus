# Plant Health + (Plus)

A desktop plant-tracking software that provides the ability to track, monitor, and log plants.

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [How to Use](#how-to-use)
3. [Troubleshooting](#troubleshooting)
3. [Contributors](#contributors)


## Prerequisites

- Qt 6.x
- CMake 3.16 or higher
- A compatible C++ compiler
- Git (though optional, as it will be used for cloning)

## How to Use
### Windows:
1. Clone the repository:

> git clone https://github.com/JeffreyHansen/Plant-Health-Plus
>
> cd PlantHealthPlus

2. Create a build directory and navigate to it:

>mkdir build
>
>cd build

3. Build the project (Release mode):

>cmake --build . --config Release

4. Open Command Prompt in the Release folder, and run windeployqt to bundle Qt dependencies.

>"C:/Qt/6.9.1/mingw_64/bin/windeployqt.exe" PlantHealthPlus.exe
### MacOS:

## Troubleshooting
### Windows:
- Missing DLLs? Run windeployqt.
- Permission issues? Run Command Prompt as Administrator.
### MacOS:

## Contributors
Jeffrey Hansen, Tal Faitelson, Daniel Hernandez Vigil, Ashley Flores

