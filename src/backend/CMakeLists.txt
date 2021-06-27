cmake_minimum_required(VERSION 3.12)

file(GLOB backend_cpp_files CONFIGURE_DEPENDS "*.cpp")

add_library( Backend STATIC
             ${backend_cpp_files}
             Converter.cpp
            )

target_link_directories(Backend PRIVATE ../../include/backend)
set_target_properties(Backend PROPERTIES LINKER_LANGUAGE CXX)
