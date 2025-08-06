# Plant Health Plus - Unit Test Suite

## Overview

This comprehensive unit test suite tests the core functionality of the Plant Health Plus application using Qt's testing framework (QtTest). The suite includes 10+ individual test methods across 5 test classes, covering critical components of the application.

## Test Coverage

### 1. PlantManager Tests (5 test methods)
- **test_addPlant()**: Verifies plants can be added correctly with proper signal emission
- **test_removePlant()**: Tests plant removal functionality and signal handling
- **test_updatePlant()**: Validates plant data updates work correctly
- **test_plantCount()**: Ensures plant counting is accurate after add/remove operations
- **test_savePlantsToFile()**: Tests data persistence to/from file system

### 2. UserManager Tests (4 test methods)
- **test_createUser()**: Tests user creation with password hashing
- **test_authenticateUser()**: Validates user authentication with correct/incorrect credentials
- **test_userExists()**: Checks user existence verification functionality
- **test_userVolume()**: Tests user-specific volume settings storage/retrieval

### 3. PlantCard Tests (2 test methods)
- **test_plantDataStorage()**: Verifies plant data is correctly stored and retrieved from UI components
- **test_expansion()**: Tests UI expansion/collapse functionality

### 4. RangeIndicator Tests (4 test methods)
- **test_construction()**: Verifies widget creation and initialization
- **test_setRange()**: Tests setting min/max range values
- **test_setCurrentValue()**: Validates current value setting within/outside ranges
- **test_compactMode()**: Tests compact display mode toggling

### 5. LogbookManager Tests (2 test methods)
- **test_addEntry()**: Tests logbook entry creation and validation
- **test_entryConstruction()**: Verifies proper construction of logbook entries with timestamps

## Running the Tests

### Method 1: Using the Test Runner Script (Recommended)
```bash
cd PlantHealthPlus/tests
./run_tests.sh
```

### Method 2: Manual Build and Run
```bash
cd PlantHealthPlus/tests
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="/opt/homebrew/opt/qt@6/lib/cmake" ..
cmake --build .
./PlantHealthPlus_Tests
```

## Output

The test suite provides output in two formats:

### 1. Console Output
- Real-time test execution status
- Colored output for easy reading
- Summary statistics at the end

### 2. Text File Output (test_results.txt)
- Detailed test results with timestamps
- Pass/fail status for each test class
- Summary statistics including success rate
- Test coverage information

## Test Features

- **Isolation**: Each test runs in isolation with proper setup/teardown
- **Signal Testing**: Uses QSignalSpy to verify Qt signal emissions
- **Temporary Data**: Uses QTemporaryDir for safe test data handling
- **Comprehensive Coverage**: Tests both success and failure scenarios
- **Documentation**: Each test is well-documented and self-explanatory

## Dependencies

- Qt6 Core
- Qt6 Test
- Qt6 Widgets
- Qt6 Network
- Qt6 Multimedia
- CMake 3.16+

## Test Architecture

The test suite follows Qt's unit testing best practices:
- Each test class inherits from QObject
- Uses QtTest macros for assertions (QVERIFY, QCOMPARE)
- Implements proper test lifecycle methods (init, cleanup, initTestCase, cleanupTestCase)
- Includes MOC generation for Qt's meta-object system

## Adding New Tests

To add new tests:
1. Create a new test file following the naming pattern `test_[component].cpp`
2. Add the test file to `CMakeLists.txt`
3. Include it in `test_runner.cpp`
4. Follow the existing test structure and Qt testing conventions

## Continuous Integration

This test suite can be easily integrated into CI/CD pipelines:
- Returns appropriate exit codes (0 for success, 1 for failure)
- Generates machine-readable output
- Supports automated test result parsing

## Example Test Result Output

```
=================================
Plant Health Plus - Unit Test Results
Generated: Mon Aug  6 14:30:15 2025
=================================

Test: PlantManager Tests (5 test methods) - PASSED
Test: UserManager Tests (4 test methods) - PASSED
Test: PlantCard Tests (2 test methods) - PASSED
Test: RangeIndicator Tests (4 test methods) - PASSED
Test: LogbookManager Tests (2 test methods) - PASSED

=================================
Summary:
Total Tests: 5
Passed: 5
Failed: 0
Success Rate: 100%
=================================
```

## Test Results are stored in build/test_results.txt