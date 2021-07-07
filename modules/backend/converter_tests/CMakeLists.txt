cmake_minimum_required(VERSION 3.5)

project(ConverterTests LANGUAGES CXX)

##qt service stage 1

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED)
find_package(Qt5 COMPONENTS Core Quick SerialPort REQUIRED)

##qt service stage 1 END


##generated stuff

add_definitions(-DGTEST_LANGUAGE_CXX11)

find_package(Threads REQUIRED)

if ($ENV{GOOGLETEST_DIR})
    set(GOOGLETEST_DIR $ENV{GOOGLETEST_DIR})
else ()
    if (NOT "/home/damiano/Projects/unit_test_visualizer/extern/googletest" STREQUAL "")
        message(WARNING "Using googletest src dir specified at Qt Creator wizard")
    endif ()
    set(GOOGLETEST_DIR "/home/damiano/Projects/unit_test_visualizer/extern/googletest")
endif ()
if (EXISTS ${GOOGLETEST_DIR})
    set(GTestSrc ${GOOGLETEST_DIR}/googletest)
    set(GMockSrc ${GOOGLETEST_DIR}/googlemock)
elseif (UNIX AND EXISTS /usr/src/gtest)
    set(GTestSrc /usr/src/gtest)
    message(WARNING "Using gtest from system")
    if (EXISTS /usr/src/gmock)
        set(GMockSrc /usr/src/gmock)
    endif ()
else ()
    message( FATAL_ERROR "No googletest src dir found - set GOOGLETEST_DIR to enable!")
endif ()

set(GTestFiles ${GTestSrc}/src/gtest-all.cc)
set(GTestIncludes ${GTestSrc} ${GTestSrc}/include)
if (NOT ${GMockSrc} STREQUAL "")
    list(APPEND GTestFiles ${GMockSrc}/src/gmock-all.cc)
    list(APPEND GTestIncludes ${GMockSrc} ${GMockSrc}/include)
endif ()

include_directories(${GTestIncludes})

##generated stuff END

##tested .cpp files

add_subdirectory("../converter" build)

##tested .cpp files END


##qt service stage 2

add_executable(DataHandlerTests
    DataHandler_TESTS.cpp
    DataHandler_TESTS.hpp
    ../Converter.hpp
               ${GTestFiles})

target_include_directories(DataHandlerTests PRIVATE ../)

target_compile_definitions(DataHandlerTests
   PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

add_test(NAME DataHandlerTests COMMAND DataHandlerTests)
target_link_libraries(DataHandlerTests PRIVATE Threads::Threads Qt5::Core Qt5::Quick Qt5::SerialPort Converter)

##qt service stage 2 END

