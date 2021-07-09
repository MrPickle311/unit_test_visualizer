set(global_public_headers
    ProgramObject.hpp)

add_subdirectory(program_object)

add_library(GLOBAL_LIBRARIES STATIC
            ${ProgramObject}
            ${global_public_headers})

set_target_properties(GLOBAL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
