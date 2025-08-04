cmake_minimum_required(VERSION 3.16)

project(PlantHealthPlus VERSION 0.2.11 LANGUAGES CXX)

if(NOT DEFINED CMAKE_PREFIX_PATH)
	if(APPLE)
		set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@6/lib/cmake")
	elseif(WIN32)
		set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/mingw_64/lib/cmake")
	endif()
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network Charts Multimedia)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        scraper.cpp
        scraper.h
        plantcard.cpp
        plantcard.h
        plantmanager.cpp
        plantmanager.h
        addplantdialog.cpp
        addplantdialog.h
        rangeindicator.cpp
        rangeindicator.h
        conditionchart.cpp
        conditionchart.h
        forecastclient.cpp
        forecastclient.h
        logindialog.cpp
        logindialog.h
        usermanager.cpp
        usermanager.h
        logbookmanager.cpp
        logbookmanager.h
        logbookentrydialog.cpp
        logbookentrydialog.h
)

if(DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_add_executable(PlantHealthPlus
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        images.qrc
        resources.qrc
        images/PHPlus_logo.icns
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PlantHealthPlus APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PlantHealthPlus SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PlantHealthPlus
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PlantHealthPlus PRIVATE 
	Qt${QT_VERSION_MAJOR}::Widgets
        Qt6::Network
        Qt6::Charts
        Qt6::Multimedia
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(APPLE)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.planthealthplus.app)
endif()

set_target_properties(PlantHealthPlus PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_ICON_FILE PHPlus_logo.icns
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PlantHealthPlus
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Copy the icon file to the bundle Resources directory
if(APPLE AND DEFINED QT_VERSION_MAJOR AND QT_VERSION_MAJOR EQUAL 6)
    set(ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/images/PHPlus_logo.icns")
    set_source_files_properties(${ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(PlantHealthPlus PRIVATE ${ICON_FILE})

    qt_finalize_executable(PlantHealthPlus)
endif()