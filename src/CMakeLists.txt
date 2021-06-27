macro(SUBDIRLIST result curdir)
    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

## .cpp files
subdirlist(SUBDIRS_SRC ${CMAKE_CURRENT_SOURCE_DIR})

foreach(SUBDIR ${SUBDIRS_SRC})
    add_subdirectory(${SUBDIR})
endforeach()

add_library(ALL_LIBRARIES STATIC)

##.hpp files
subdirlist(SUBDIRS_INC ${CMAKE_SOURCE_DIR}/include)

foreach(SUBDIR ${SUBDIRS_INC})
    target_include_directories(ALL_LIBRARIES PRIVATE ${SUBDIR})
endforeach()

target_link_libraries(ALL_LIBRARIES Backend)

set_target_properties(ALL_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
