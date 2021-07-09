set(global_private_headers
    )

set(sources
    ProgramObject.cpp)

add_library(ProgramObject STATIC
    ${global_private_headers}
	${sources})

target_link_libraries(ProgramObject Qt5::Core Qt5::Quick Qt5::SerialPort)
set_target_properties(ProgramObject PROPERTIES LINKER_LANGUAGE CXX)
