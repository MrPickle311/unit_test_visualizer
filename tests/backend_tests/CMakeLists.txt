cmake_minimum_required(VERSION 3.5)

project(test1 LANGUAGES CXX)

find_package(QT NAMES Qt5 COMPONENTS Test Core Quick SerialPort REQUIRED)
find_package(Qt5 COMPONENTS Test Core Quick SerialPort REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(TO_TEST_SUBDIRS backend)

set(SRC ../../src/backend/Converter.cpp)

set(INC ../../include/backend)

include_directories(test1 PRIVATE ${INC})

add_executable(test1 tst_test1.cpp ${SRC})
add_test(NAME test1 COMMAND test1)

target_link_libraries(test1 PRIVATE Qt5::Core Qt5::Quick Qt5::Test Qt5::SerialPort )

