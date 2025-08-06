cmake_minimum_required(VERSION 3.16)

project(PlantHealthPlus_Tests)

# Find Qt6 components needed for testing
find_package(Qt6 REQUIRED COMPONENTS Core Test Widgets Network Multimedia)

# Enable Qt's automatic moc, uic, and rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include parent directory for accessing main application headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

# Main application source files that tests depend on
set(APP_SOURCES
    ../plantmanager.cpp
    ../usermanager.cpp
    ../plantcard.cpp
    ../rangeindicator.cpp
    ../logbookmanager.cpp
)

# Create the comprehensive test executable
add_executable(PlantHealthPlus_ComprehensiveTests 
    comprehensive_tests.cpp
    ${APP_SOURCES}
)

# Link Qt libraries
target_link_libraries(PlantHealthPlus_ComprehensiveTests
    Qt6::Core
    Qt6::Test
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
)

# Add compile definitions
target_compile_definitions(PlantHealthPlus_ComprehensiveTests PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")

# Set output directory
set_target_properties(PlantHealthPlus_ComprehensiveTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Create the test results generator
add_executable(TestResultsGenerator 
    test_runner.cpp
)

target_link_libraries(TestResultsGenerator
    Qt6::Core
)

# Enable testing
enable_testing()

# Add the test to CTest
add_test(NAME PlantHealthPlus_UnitTests COMMAND PlantHealthPlus_ComprehensiveTests)

# Create a custom target to run tests and generate reports
add_custom_target(run_tests
    COMMAND PlantHealthPlus_ComprehensiveTests
    COMMAND TestResultsGenerator
    DEPENDS PlantHealthPlus_ComprehensiveTests TestResultsGenerator
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Plant Health Plus Unit Tests"
)
