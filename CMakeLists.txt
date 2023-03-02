cmake_minimum_required(VERSION 3.13)

project(gaffa LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

if(DEFINED STAGING_DIR)
    add_compile_definitions(GAFFA_IPC_SOCKET_GROUP="network")
    include_directories(${STAGING_DIR}/usr/include/libnl3)
else()
    add_compile_definitions(GAFFA_IPC_SOCKET_GROUP="root")
    include_directories(/usr/include/libnl3)
endif()

add_executable(gaffa
        src/ipc/Socket.cpp src/main.cpp
        src/ipc/Subscriber.cpp
        src/ipc/Subscriber.h
        src/ipc/Queue.cpp
        src/ipc/Queue.h
        src/ipc/Event.cpp
        src/ipc/Event.h
        src/ipc/AssocEvent.cpp
        src/ipc/AssocEvent.h
        src/ipc/AuthEvent.cpp
        src/ipc/AuthEvent.h
        src/ipc/DisassocEvent.cpp
        src/ipc/DisassocEvent.h
        src/ipc/Caller.cpp
        src/ipc/Caller.h
        src/nl/Socket.cpp
        src/nl/Socket.h
        src/nl/Vxlan.cpp
        src/nl/Vxlan.h
        src/nl/Bridge.cpp
        src/nl/Bridge.h
        src/nl/Link.cpp
        src/nl/Link.h
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
        src/ipc/TimeoutException.cpp src/ipc/TimeoutException.h
        src/ConfigParser.cpp
        src/ConfigParser.h)
target_link_libraries(gaffa nl-3 nl-route-3)

include_directories(src/hostapd)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
