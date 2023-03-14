cmake_minimum_required(VERSION 3.13)

project(gaffa LANGUAGES CXX)

find_package(prometheus-cpp CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")


add_compile_definitions(ELPP_THREAD_SAFE)
add_compile_definitions(ELPP_FEATURE_CRASH_LOG)

if(DEFINED STAGING_DIR)
    add_compile_definitions(GAFFA_IPC_SOCKET_GROUP="network")
    include_directories(${STAGING_DIR}/usr/include/libnl3)
    add_compile_definitions(ELPP_SYSLOG)
else()
    add_compile_definitions(GAFFA_IPC_SOCKET_GROUP="root")
    include_directories(/usr/include/libnl3)
endif()

add_executable(gaffa
        src/ipc/Socket.cpp src/main.cpp
        src/SynchronizedQueue.cpp
        src/SynchronizedQueue.h
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
        src/NetworkRenderer.cpp
        src/NetworkRenderer.h
        src/BridgePerVxlanRenderer.cpp
        src/BridgePerVxlanRenderer.h
        src/Station.cpp
        src/Station.h
        src/TimeoutException.cpp
        src/TimeoutException.h
        src/nl/Subscriber.cpp
        src/nl/Subscriber.h
        src/VlanMissingException.cpp
        src/VlanMissingException.h
        src/logging/easylogging++.cpp
        src/logging/easylogging++.h
        src/ConfigParser.cpp
        src/ConfigParser.h
        src/metrics/MetricsManager.cpp
        src/metrics/MetricsManager.h
        src/logging/loginit.h
        src/logging/loginit.h
        src/Configuration.cpp
        src/Configuration.h
        src/nl/Socket80211.cpp
        src/nl/Socket80211.h)
target_link_libraries(gaffa nl-3 nl-route-3 nl-genl-3 prometheus-cpp::pull prometheus-cpp::core)

include_directories(src/hostapd)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
