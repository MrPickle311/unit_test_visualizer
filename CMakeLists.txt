cmake_minimum_required(VERSION 3.14)

project(uart_unit_test_visualizer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt5 REQUIRED)
find_package(Qt5 COMPONENTS Core Quick SerialPort REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(uart_unit_test_visualizer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(uart_unit_test_visualizer SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(uart_unit_test_visualizer
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(uart_unit_test_visualizer
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(uart_unit_test_visualizer Qt5::Core Qt5::Quick Qt5::SerialPort)

set_target_properties(uart_unit_test_visualizer PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(uart_unit_test_visualizer)
    qt_finalize_executable(uart_unit_test_visualizer)
endif()
