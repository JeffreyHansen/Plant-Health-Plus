#!/bin/bash

# Plant Health Plus Build Script
# Automatically detects OS and builds the project

set -e  # Exit on any error

# Colors for output
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

# Detect OS
detect_os() {
    case "$(uname -s)" in
        Darwin*)
            OS="macOS"
            ;;
        Linux*)
            OS="Linux"
            ;;
        MINGW*|MSYS*|CYGWIN*)
            OS="Windows"
            ;;
        *)
            print_error "Unsupported operating system: $(uname -s)"
            exit 1
            ;;
    esac
    print_status "Detected OS: $OS"
}

# Check prerequisites
check_prerequisites() {
    print_status "Checking prerequisites..."
    
    # Check CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake is not installed. Please install CMake 3.16 or higher."
        exit 1
    fi
    
    CMAKE_VERSION=$(cmake --version | head -n1 | grep -o '[0-9]\+\.[0-9]\+\.[0-9]\+')
    print_status "Found CMake version: $CMAKE_VERSION"
    
    # Check Git
    if ! command -v git &> /dev/null; then
        print_warning "Git is not installed. This is optional but recommended."
    fi
    
    # OS-specific checks
    case $OS in
        "macOS")
            # Check for Qt via Homebrew
            if ! brew list qt@6 &> /dev/null; then
                print_warning "Qt6 not found via Homebrew. Attempting to install..."
                brew install qt@6
            fi
            
            # Check for Xcode Command Line Tools
            if ! xcode-select -p &> /dev/null; then
                print_error "Xcode Command Line Tools not installed. Please run: xcode-select --install"
                exit 1
            fi
            ;;
        "Windows"|"Linux")
            print_status "Please ensure Qt6 is installed and in your PATH"
            ;;
    esac
}

# Build function
build_project() {
    print_status "Starting build process..."
    
    # Navigate to PlantHealthPlus directory
    if [ ! -d "PlantHealthPlus" ]; then
        print_error "PlantHealthPlus directory not found. Are you in the correct directory?"
        exit 1
    fi
    
    cd PlantHealthPlus
    
    # Create build directory
    print_status "Creating build directory..."
    mkdir -p build
    cd build
    
    # Configure with CMake
    print_status "Configuring project with CMake..."
    if ! cmake ..; then
        print_error "CMake configuration failed"
        exit 1
    fi
    
    # Build the project
    print_status "Building project..."
    if ! cmake --build . --config Release; then
        print_error "Build failed"
        exit 1
    fi
    
    # OS-specific deployment
    case $OS in
        "macOS")
            print_status "Deploying macOS application..."
            if [ -f "PlantHealthPlus.app/Contents/MacOS/PlantHealthPlus" ]; then
                # Check if the app bundle has a proper Info.plist
                if [ ! -f "PlantHealthPlus.app/Contents/Info.plist" ]; then
                    print_error "Info.plist not found in app bundle"
                    exit 1
                fi
                
                # Find macdeployqt
                MACDEPLOYQT_PATH="/opt/homebrew/opt/qt@6/bin/macdeployqt"
                if [ ! -f "$MACDEPLOYQT_PATH" ]; then
                    # Try alternative path
                    MACDEPLOYQT_PATH="/usr/local/opt/qt@6/bin/macdeployqt"
                fi
                
                if [ -f "$MACDEPLOYQT_PATH" ]; then
                    print_status "Running macdeployqt (warnings about rpath are normal)..."
                    "$MACDEPLOYQT_PATH" PlantHealthPlus.app 2>/dev/null || true
                    print_success "Qt frameworks deployed to app bundle"
                else
                    print_warning "macdeployqt not found. App bundle may be missing dependencies."
                fi
                
                # Copy the icon file to Resources directory if it's missing
                if [ ! -f "PlantHealthPlus.app/Contents/Resources/PHPlus_logo.icns" ]; then
                    if [ -f "../images/PHPlus_logo.icns" ]; then
                        print_status "Copying application icon..."
                        cp "../images/PHPlus_logo.icns" "PlantHealthPlus.app/Contents/Resources/"
                        print_success "Application icon copied"
                    else
                        print_warning "Application icon not found at ../images/PHPlus_logo.icns"
                    fi
                fi
                
                # Code sign the application
                print_status "Code signing the application..."
                if command -v codesign &> /dev/null; then
                    # Remove extended attributes that might interfere with signing
                    xattr -cr PlantHealthPlus.app 2>/dev/null || true
                    
                    # Sign the application with ad-hoc signature (for local development)
                    if codesign --force --deep --sign - PlantHealthPlus.app 2>/dev/null; then
                        print_success "Application signed successfully"
                        
                        # Verify the signature
                        if codesign --verify --deep --strict PlantHealthPlus.app 2>/dev/null; then
                            print_success "Code signature verified"
                        else
                            print_warning "Code signature verification failed, but app should still work"
                        fi
                    else
                        print_error "Code signing failed. The app may not launch due to macOS security restrictions."
                        print_status "You can try running: codesign --force --deep --sign - PlantHealthPlus.app"
                    fi
                else
                    print_error "codesign tool not found. App will likely fail to launch on macOS."
                fi
                
                print_success "macOS app bundle created successfully!"
                print_status "You can run the app with: open PlantHealthPlus.app"
                print_status "Location: $(pwd)/PlantHealthPlus.app"
                
            else
                print_error "PlantHealthPlus.app not found after build"
                exit 1
            fi
            ;;
        "Windows")
            print_status "Deploying Windows application..."
            if [ -f "Release/PlantHealthPlus.exe" ]; then
                cd Release
                # Try to find windeployqt
                WINDEPLOYQT_PATHS=(
                    "C:/Qt/6.9.1/mingw_64/bin/windeployqt.exe"
                    "C:/Qt/6.8.0/mingw_64/bin/windeployqt.exe"
                    "C:/Qt/6.7.0/mingw_64/bin/windeployqt.exe"
                )
                
                WINDEPLOYQT_FOUND=false
                for path in "${WINDEPLOYQT_PATHS[@]}"; do
                    if [ -f "$path" ]; then
                        "$path" PlantHealthPlus.exe
                        WINDEPLOYQT_FOUND=true
                        break
                    fi
                done
                
                if [ "$WINDEPLOYQT_FOUND" = true ]; then
                    print_success "Windows executable created successfully!"
                    print_status "You can run the app with: ./PlantHealthPlus.exe"
                else
                    print_warning "windeployqt not found. Executable may be missing dependencies."
                    print_status "Please run windeployqt manually from your Qt installation."
                fi
                cd ..
            else
                print_error "PlantHealthPlus.exe not found after build"
                exit 1
            fi
            ;;
        "Linux")
            print_status "Linux build completed."
            if [ -f "PlantHealthPlus" ]; then
                print_success "Linux executable created successfully!"
                print_status "You can run the app with: ./PlantHealthPlus"
            else
                print_error "PlantHealthPlus executable not found after build"
                exit 1
            fi
            ;;
    esac
}

# Main execution
main() {
    print_status "Plant Health Plus Build Script"
    print_status "=============================="
    
    detect_os
    check_prerequisites
    build_project
    
    print_success "Build completed successfully!"
    print_status "Check the build/Release directory (Windows) or build directory (macOS/Linux) for your executable."
}

# Run main function
main "$@"
