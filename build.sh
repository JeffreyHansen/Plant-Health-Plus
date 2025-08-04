#!/bin/bash

# Plant Health Plus - macOS Build Script
# Automatically builds and deploys the Plant Health Plus application

set -e  # Exit on any error

echo "🌱 Plant Health Plus - macOS Build Script"
echo "========================================"

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're on macOS
if [[ "$OSTYPE" != "darwin"* ]]; then
    print_error "This script is for macOS only. Use build.bat for Windows."
    exit 1
fi

# Navigate to the project directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$SCRIPT_DIR/PlantHealthPlus"

if [ ! -d "$PROJECT_DIR" ]; then
    print_error "PlantHealthPlus directory not found. Make sure you're running this from the project root."
    exit 1
fi

cd "$PROJECT_DIR"

print_status "Checking prerequisites..."

# Check for CMake
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Install with: brew install cmake"
    exit 1
fi

CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
print_status "Found CMake version: $CMAKE_VERSION"

# Check for Qt6 - try multiple common paths
QT_PATHS=(
    "/opt/homebrew/opt/qt@6"
    "/usr/local/opt/qt@6"
    "/opt/homebrew/opt/qt"
    "/usr/local/opt/qt"
)

QT_PATH=""
for path in "${QT_PATHS[@]}"; do
    if [ -d "$path" ]; then
        QT_PATH="$path"
        break
    fi
done

if [ -z "$QT_PATH" ]; then
    print_error "Qt6 not found in any of the expected locations:"
    for path in "${QT_PATHS[@]}"; do
        print_error "  $path"
    done
    print_error "Install Qt6 with: brew install qt@6"
    exit 1
fi

print_status "Found Qt6 at: $QT_PATH"

# Verify Qt version
QT_VERSION=$(cat "$QT_PATH/lib/cmake/Qt6/Qt6ConfigVersion.cmake" 2>/dev/null | grep "set(PACKAGE_VERSION" | cut -d'"' -f2)
if [ -n "$QT_VERSION" ]; then
    print_status "Qt6 version: $QT_VERSION"
fi

# Check for Xcode Command Line Tools
if ! xcode-select -p &> /dev/null; then
    print_error "Xcode Command Line Tools not found. Install with: xcode-select --install"
    exit 1
fi

print_status "Xcode Command Line Tools found"

# Check for vcpkg dependencies if vcpkg.json exists
if [ -f "vcpkg.json" ]; then
    print_status "Found vcpkg.json - checking vcpkg dependencies..."
    if command -v vcpkg &> /dev/null; then
        print_status "vcpkg is available"
    else
        print_warning "vcpkg not found in PATH. Dependencies might need manual installation."
    fi
fi

print_status "Starting build process..."

# Clean previous build
if [ -d "build" ]; then
    print_status "Cleaning previous build..."
    rm -rf build
fi

# Create build directory
print_status "Creating build directory..."
mkdir build
cd build

# Configure project with CMake
print_status "Configuring project with CMake..."
cmake -DCMAKE_PREFIX_PATH="$QT_PATH/lib/cmake" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_DEPLOYMENT_TARGET=10.15 \
      ..

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed"
    exit 1
fi

# Build the project
print_status "Building project..."
NUM_CORES=$(sysctl -n hw.ncpu)
print_status "Using $NUM_CORES parallel jobs"

cmake --build . --config Release --parallel $NUM_CORES

if [ $? -ne 0 ]; then
    print_error "Build failed"
    exit 1
fi

print_success "Build completed successfully!"

# Deploy Qt dependencies
print_status "Deploying Qt dependencies..."
MACDEPLOYQT="$QT_PATH/bin/macdeployqt"

if [ ! -f "$MACDEPLOYQT" ]; then
    print_error "macdeployqt not found at $MACDEPLOYQT"
    exit 1
fi

print_status "Running macdeployqt (warnings about rpath are normal)..."
"$MACDEPLOYQT" PlantHealthPlus.app -verbose=2

if [ $? -ne 0 ]; then
    print_warning "macdeployqt completed with warnings (this is usually normal)"
else
    print_success "Qt frameworks deployed successfully"
fi

# Code sign the application
print_status "Code signing the application..."
codesign --force --deep --sign - PlantHealthPlus.app

if [ $? -eq 0 ]; then
    print_success "Application signed successfully"
    
    # Verify code signature
    if codesign --verify --deep --strict PlantHealthPlus.app; then
        print_success "Code signature verified"
    else
        print_warning "Code signature verification failed, but app should still run"
    fi
else
    print_warning "Code signing failed, but app should still run locally"
fi

# Copy icon to app bundle (if needed)
ICON_SOURCE="../images/PHPlus_logo.icns"
ICON_DEST="PlantHealthPlus.app/Contents/Resources/PHPlus_logo.icns"

if [ -f "$ICON_SOURCE" ] && [ ! -f "$ICON_DEST" ]; then
    print_status "Copying application icon..."
    mkdir -p "$(dirname "$ICON_DEST")"
    cp "$ICON_SOURCE" "$ICON_DEST"
    print_success "Application icon copied"
fi

# Final checks
if [ -d "PlantHealthPlus.app" ]; then
    APP_SIZE=$(du -sh PlantHealthPlus.app | cut -f1)
    print_success "macOS app bundle created successfully!"
    print_status "App bundle size: $APP_SIZE"
    print_status "Location: $(pwd)/PlantHealthPlus.app"
    
    # Check if the app is actually executable
    if [ -x "PlantHealthPlus.app/Contents/MacOS/PlantHealthPlus" ]; then
        print_success "Executable binary found and is executable"
    else
        print_warning "Executable binary not found or not executable"
    fi
    
    echo ""
    print_success "🚀 Build completed successfully!"
    echo ""
    echo "To run the application:"
    echo "  open PlantHealthPlus.app"
    echo ""
    echo "To test the application:"
    echo "  ./PlantHealthPlus.app/Contents/MacOS/PlantHealthPlus"
    echo ""
    echo "To install in Applications folder:"
    echo "  cp -r PlantHealthPlus.app /Applications/"
    echo ""
    echo "To distribute:"
    echo "  Create a .dmg or zip the PlantHealthPlus.app bundle"
else
    print_error "App bundle not found. Build may have failed."
    exit 1
fi