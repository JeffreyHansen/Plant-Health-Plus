#!/bin/bash

# Plant Health Plus - Unit Test Runner
# Builds and runs unit tests, outputs results to console and file

set -e  # Exit on any error

echo "🧪 Plant Health Plus - Unit Test Runner"
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

# Navigate to test directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR"

print_status "Current directory: $PWD"

# Check for Qt6
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
    print_error "Qt6 not found in any of the expected locations"
    exit 1
fi

print_status "Found Qt6 at: $QT_PATH"

# Clean previous build
if [ -d "build" ]; then
    print_status "Cleaning previous test build..."
    rm -rf build
fi

# Create build directory
print_status "Creating test build directory..."
mkdir build
cd build

# Configure tests with CMake
print_status "Configuring tests with CMake..."
cmake -DCMAKE_PREFIX_PATH="$QT_PATH/lib/cmake" \
      -DCMAKE_BUILD_TYPE=Debug \
      ..

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed"
    exit 1
fi

# Build the tests
print_status "Building tests..."
cmake --build . --config Debug --parallel $(sysctl -n hw.ncpu)

if [ $? -ne 0 ]; then
    print_error "Test build failed"
    exit 1
fi

print_success "Tests built successfully!"

# Run the tests
print_status "Running unit tests..."
echo ""

./PlantHealthPlus_ComprehensiveTests

TEST_EXIT_CODE=$?

echo ""

# Run the test results generator
print_status "Generating test results summary..."
./TestResultsGenerator

if [ $TEST_EXIT_CODE -eq 0 ]; then
    print_success "All tests passed!"
else
    print_warning "Some tests failed (exit code: $TEST_EXIT_CODE)"
fi

# Check if test results file was created
if [ -f "test_results.txt" ]; then
    print_success "Test results written to: $(pwd)/test_results.txt"
    
    # Also copy to parent directory for easy access
    cp test_results.txt ..
    print_status "Test results copied to: $(dirname "$PWD")/test_results.txt"
    
    echo ""
    print_status "Test Results Summary:"
    echo "===================="
    tail -8 test_results.txt
else
    print_warning "Test results file not found"
fi

echo ""
if [ $TEST_EXIT_CODE -eq 0 ]; then
    print_success "🎉 Unit testing completed successfully!"
else
    print_warning "⚠️ Unit testing completed with failures"
fi

exit $TEST_EXIT_CODE
