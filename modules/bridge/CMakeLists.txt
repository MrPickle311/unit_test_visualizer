set(bridge_public_headers
    Bridge.hpp
    SettingsBridge.hpp
    TerminalBridge.hpp
    TestsBridge.hpp
    ErrorReporter.hpp )

set(bridge_sources
    SettingsBridge.cpp
    ErrorReporter.cpp)

add_library(BRIDGE_LIBRARIES STATIC
            ${bridge_public_headers}
            ${bridge_sources})

target_link_libraries(BRIDGE_LIBRARIES Qt5::Core Qt5::Quick Qt5::SerialPort Port ProgramObject Parser Converter)
set_target_properties(BRIDGE_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
