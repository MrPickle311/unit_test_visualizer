cmake_minimum_required(VERSION 3.12)

file(GLOB backend_cpp_files CONFIGURE_DEPENDS "*.cpp")

add_library( Backend
             ${backend_cpp_files}
            )

target_link_libraries(Backend Qt5::Core Qt5::Quick Qt5::SerialPort)
target_include_directories(Backend PRIVATE ../../include/backend)
set_target_properties(Backend PROPERTIES LINKER_LANGUAGE CXX)
