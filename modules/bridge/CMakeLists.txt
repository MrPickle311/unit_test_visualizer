set(bridge_public_headers
    Bridge.hpp
    SettingsBridge.hpp
    TerminalBridge.hpp )

set(bridge_sources
    Bridge.cpp
     SettingsBridge.cpp
     TerminalBridge.cpp)

add_library(BRIDGE_LIBRARIES STATIC
            ${bridge_public_headers}
            ${bridge_sources})

target_link_libraries(BRIDGE_LIBRARIES Qt5::Core Qt5::Quick Qt5::SerialPort Port ProgramObject)
set_target_properties(BRIDGE_LIBRARIES PROPERTIES LINKER_LANGUAGE CXX)
