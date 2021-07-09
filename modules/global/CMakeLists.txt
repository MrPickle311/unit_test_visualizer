set(global_public_headers
    )

add_subdirectory(sources)

target_include_directories(GLOBAL_LIBRARIES PUBLIC ${global_public_headers})
set_target_properties(GLOBAL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
