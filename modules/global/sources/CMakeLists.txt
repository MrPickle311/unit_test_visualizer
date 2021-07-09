set(global_private_headers
    )

add_library(GLOBAL_LIBRARIES STATIC
    ${global_private_headers})

target_link_libraries(GLOBAL_LIBRARIES Qt5::Core Qt5::Quick Qt5::SerialPort)
set_target_properties(GLOBAL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
