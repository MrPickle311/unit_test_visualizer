add_subdirectory(port)
add_subdirectory(parser)
add_subdirectory(converter)

set(backend_public_headers
    PortOperator.hpp
    PortSettings.hpp
    Parser.hpp
    Converter.hpp
    ConvertedDataTypes.hpp
    ParsedDataTypes.hpp
    PortScanner.hpp
    Interfaces.hpp)

add_library(BACKEND_LIBRARIES STATIC
    ${backend_public_headers})

target_link_libraries(BACKEND_LIBRARIES Port Parser Converter)
target_link_libraries(BACKEND_LIBRARIES Qt5::Core Qt5::Quick Qt5::SerialPort)
set_target_properties(BACKEND_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
