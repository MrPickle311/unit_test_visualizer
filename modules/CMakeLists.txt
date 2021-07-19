add_library(ALL_LIBRARIES STATIC)

add_subdirectory(backend)
add_subdirectory(global)

target_link_libraries(ALL_LIBRARIES BACKEND_LIBRARIES GLOBAL_LIBRARIES)

set_target_properties(ALL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
