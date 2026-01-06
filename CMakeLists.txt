cmake_minimum_required(VERSION 3.16)

project(os_kurs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(os_kurs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}


    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET os_kurs APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(os_kurs SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(os_kurs
            ${PROJECT_SOURCES}
        )
    endif()
endif()
# Добавляем исходники midifile к проекту
target_sources(os_kurs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/src/MidiFile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/src/Binasc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/src/MidiEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/src/MidiEventList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/src/MidiMessage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/src/Options.cpp
)


# Добавляем путь к include
target_include_directories(os_kurs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/midifile/include
)
target_sources(os_kurs PRIVATE
    rtmidi-master/RtMidi.cpp
)
target_include_directories(os_kurs PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/rtmidi-master
)
target_link_libraries(os_kurs PRIVATE winmm)

target_link_libraries(os_kurs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
if(WIN32)
    target_link_libraries(os_kurs PRIVATE winmm)
endif()
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.os_kurs)
endif()
set_target_properties(os_kurs PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS os_kurs
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(os_kurs)
endif()
