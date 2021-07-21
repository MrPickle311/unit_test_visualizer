##adding subdirectories automatically
##macro(SUBDIRLIST result curdir)
##    file(GLOB children RELATIVE ${curdir} ${curdir}/*)
##    set(dirlist "")
##    foreach(child ${children})
##        if(IS_DIRECTORY ${curdir}/${child})
##            list(APPEND dirlist ${child})
##        endif()
##    endforeach()
##    set(${result} ${dirlist})
##endmacro()
##
##subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
##
##foreach(SUBDIR ${SUBDIRS})
##    add_subdirectory(${SUBDIR})
##endforeach()
##adding subdirectories automatically END

add_subdirectory(port)
add_subdirectory(parser)
add_subdirectory(converter)

set(backend_public_headers
    PortOperator.hpp
    PortSettings.hpp
    Parser.hpp
    Converter.hpp)

add_library(BACKEND_LIBRARIES STATIC
    ${backend_public_headers})

target_link_libraries(BACKEND_LIBRARIES Port Parser Converter)
target_link_libraries(BACKEND_LIBRARIES Qt5::Core Qt5::Quick Qt5::SerialPort)
set_target_properties(BACKEND_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
