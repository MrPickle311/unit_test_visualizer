cmake_minimum_required(VERSION 3.12)

set(converter_cpp_files
        Converter.cpp)

set(conveter_private_hpp_files
    )

add_library( Converter
             ${converter_cpp_files}
             ${conveter_private_hpp_files}
            )

target_link_libraries(Converter Qt5::Core Qt5::Quick Qt5::SerialPort ProgramObject)

set_target_properties(Converter PROPERTIES LINKER_LANGUAGE CXX)
