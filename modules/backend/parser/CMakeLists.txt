cmake_minimum_required(VERSION 3.12)

set(parser_cpp_files
        Parser.cpp)

set(parser_private_hpp_files )

add_library( Parser
             ${parser_cpp_files}
             ${parser_private_hpp_files}
            )

target_link_libraries(Parser Qt5::Core Qt5::Quick Qt5::SerialPort ProgramObject)

set_target_properties(Parser PROPERTIES LINKER_LANGUAGE CXX)
