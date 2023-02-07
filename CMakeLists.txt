cmake_minimum_required(VERSION 3.13)

project(gaffa LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
add_definitions(-DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX)
add_executable(gaffa src/hostapd/os_unix.c src/hostapd/wpa_ctrl.c src/IPCSocket.cpp src/main.cpp src/IPCSubscriber.cpp src/IPCSubscriber.h src/IPCQueue.cpp src/IPCQueue.h src/IPCEvent.cpp src/IPCEvent.h src/EventLoop.cpp src/EventLoop.h src/EventHandler.cpp src/EventHandler.h src/IPCAssocEvent.cpp src/IPCAssocEvent.h src/IPCAuthEvent.cpp src/IPCAuthEvent.h src/IPCDisassocEvent.cpp src/IPCDisassocEvent.h src/IPCCaller.cpp src/IPCCaller.h src/NetworkRenderer.cpp src/NetworkRenderer.h src/BridgePerVxlanRenderer.cpp src/BridgePerVxlanRenderer.h src/Station.cpp src/Station.h)

include_directories(src/hostapd)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
