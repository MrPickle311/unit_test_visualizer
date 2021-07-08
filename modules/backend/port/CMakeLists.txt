cmake_minimum_required(VERSION 3.12)

set(port_cpp_files
	PortOperator.cpp
	PortSettings.cpp)

set(port_private_hpp_files )

add_library( Port
             ${port_cpp_files}
             ${port_private_hpp_files}
            )

target_link_libraries(Port Qt5::Core Qt5::Quick Qt5::SerialPort)

set_target_properties(Port PROPERTIES LINKER_LANGUAGE CXX)
