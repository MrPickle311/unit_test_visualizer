cmake_minimum_required(VERSION 3.12)

set(converter_cpp_files Converter.cpp)

set(converter_public_hpp_files ../Converter.hpp)

set(converter_private_hpp_files )

add_library( Converter
             ${converter_cpp_files}
             ${converter_public_hpp_files}
             ${converter_private_hpp_files}
            )

target_link_libraries(Converter Qt5::Core Qt5::Quick Qt5::SerialPort)

set_target_properties(Converter PROPERTIES LINKER_LANGUAGE CXX)
