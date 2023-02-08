cmake_minimum_required(VERSION 3.13)

project(gaffa LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
include_directories("/usr/include/libnl3/")
add_definitions(-DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX)
add_executable(gaffa
        src/ipc/IPCSocket.cpp src/main.cpp
        src/ipc/IPCSubscriber.cpp
        src/ipc/IPCSubscriber.h
        src/ipc/IPCQueue.cpp
        src/ipc/IPCQueue.h
        src/ipc/IPCEvent.cpp
        src/ipc/IPCEvent.h
        src/ipc/IPCAssocEvent.cpp
        src/ipc/IPCAssocEvent.h
        src/ipc/IPCAuthEvent.cpp
        src/ipc/IPCAuthEvent.h
        src/ipc/IPCDisassocEvent.cpp
        src/ipc/IPCDisassocEvent.h
        src/ipc/IPCCaller.cpp
        src/ipc/IPCCaller.h
        src/EventLoop.cpp
        src/EventLoop.h
        src/EventHandler.cpp
        src/EventHandler.h
        src/NetworkRenderer.cpp
        src/NetworkRenderer.h
        src/BridgePerVxlanRenderer.cpp
        src/BridgePerVxlanRenderer.h
        src/Station.cpp
        src/Station.h
        src/NetlinkSocket.cpp
        src/NetlinkSocket.h
        src/NetlinkVxlan.cpp
        src/NetlinkVxlan.h
)
target_link_libraries(gaffa /usr/lib/libnl-3.so /usr/lib/libnl-route-3.so)

include_directories(src/hostapd)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
