cmake_minimum_required(VERSION 3.16)

# Enable testing
enable_testing()

# Find Qt6 modules
find_package(Qt6 REQUIRED COMPONENTS Test Widgets Network Charts Multimedia)

# Set up test source files
set(TEST_SOURCES
    test_main.cpp
    test_plantmanager.cpp
    test_usermanager.cpp
    test_logbookmanager.cpp
    test_plantcard.cpp
    test_rangeindicator.cpp
    test_conditionchart.cpp
    test_forecastclient.cpp
    test_scraper.cpp
    test_addplantdialog.cpp
    test_logbookentrydialog.cpp
)

# Create test executable
qt_add_executable(PlantHealthPlusTests
    ${TEST_SOURCES}
)

# Link against the main application sources and Qt Test
target_link_libraries(PlantHealthPlusTests PRIVATE
    Qt6::Test
    Qt6::Widgets
    Qt6::Network
    Qt6::Charts
    Qt6::Multimedia
)

# Include the main source directory
target_include_directories(PlantHealthPlusTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Add the main application sources to the test executable
target_sources(PlantHealthPlusTests PRIVATE
    ../plantmanager.cpp
    ../usermanager.cpp
    ../logbookmanager.cpp
    ../plantcard.cpp
    ../rangeindicator.cpp
    ../conditionchart.cpp
    ../forecastclient.cpp
    ../scraper.cpp
    ../addplantdialog.cpp
    ../logbookentrydialog.cpp
)

# Add test to CTest
add_test(NAME PlantHealthPlusTests COMMAND PlantHealthPlusTests)