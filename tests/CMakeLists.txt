cmake_minimum_required(VERSION 3.5)

project(Tests LANGUAGES CXX)

##qt service stage 1

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED)
find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Quick SerialPort REQUIRED)

##qt service stage 1 END


##generated stuff

add_definitions(-DGTEST_LANGUAGE_CXX11)
set(GTEST_COLOR=1)




set(GOOGLETEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../extern/googletest")

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

add_subdirectory("../modules" build)

##tested .cpp files END

##qt service stage 2

set(LIBS
    Port
    ProgramObject
    Parser
    Converter)

set(TESTED_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/global
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend)

set(TESTED_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/global/ProgramObject.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/global/GlobalFunctions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/global/StaticGenerator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/PortSettings.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/PortOperator.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/Parser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/ParsedDataTypes.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/Converter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/Interfaces.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../modules/backend/PortScanner.hpp)

set(PortTest_files
    port/Port_UnitTests.cpp
    port/Port_UnitTests.hpp
    port/Port_IntegrationTests.cpp
    port/Port_IntegrationTests.hpp)

set(ParserTest_files
    parser/Parser_UnitTests.cpp
    parser/Parser_UnitTests.hpp
    parser/UnitTestBuilder.hpp
    parser/UnitTestBuilder.cpp
    )

set(ConverterTest_files
    converter/Converter_UnitTests.cpp
    converter/Converter_UnitTests.hpp
    converter/Converter_IntegrationTests.hpp
    converter/Converter_IntegrationTests.cpp)

set(Tests_files
    main.cpp
    common.cpp
    common.hpp
    ${PortTest_files}
    ${TESTED_HEADERS}
    ${ParserTest_files}
    ${ConverterTest_files}
    )

function(add_test_exec test_name test_files test_libs)
    add_executable(${test_name} ${test_files} ${GTestFiles})
    target_include_directories(${test_name} PUBLIC ${TESTED_DIRECTORIES})
    target_compile_definitions(${test_name}
       PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
   add_test(NAME ${test_name} COMMAND ${test_name})
   target_link_libraries(${test_name} PRIVATE
       Threads::Threads Qt5::Core Qt5::Quick Qt5::SerialPort ${test_libs})
endfunction()

add_test_exec(Tests "${Tests_files}" "${LIBS}")

##qt service stage 2 END

