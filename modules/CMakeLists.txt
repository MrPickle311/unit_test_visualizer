add_library(ALL_LIBRARIES STATIC)

add_subdirectory(backend)

target_link_libraries(ALL_LIBRARIES BACKEND_LIBRARIES)

set_target_properties(ALL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
