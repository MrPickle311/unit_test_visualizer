set(global_public_headers
    ProgramObject.hpp
    GlobalFunctions.hpp)

add_subdirectory(program_object)

add_library(GLOBAL_LIBRARIES STATIC
            ${global_public_headers})

target_link_libraries(GLOBAL_LIBRARIES ProgramObject)
set_target_properties(GLOBAL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
