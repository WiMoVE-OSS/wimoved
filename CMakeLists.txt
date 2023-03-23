cmake_minimum_required(VERSION 3.13)

project(wimoved LANGUAGES CXX)

find_package(prometheus-cpp CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(ELPP_THREAD_SAFE)
add_compile_definitions(ELPP_FEATURE_CRASH_LOG)

if(DEFINED STAGING_DIR)
    include_directories(${STAGING_DIR}/usr/include/libnl3)
    add_compile_definitions(ELPP_SYSLOG)
else()
    include_directories(/usr/include/libnl3)
endif()

add_executable(wimoved
        src/ipc/Socket.cpp src/main.cpp
        src/ipc/Subscriber.cpp
        src/ipc/Subscriber.h
        src/SynchronizedQueue.cpp
        src/SynchronizedQueue.h
        src/ipc/Event.cpp
        src/ipc/Event.h
        src/ipc/ConnectEvent.cpp
        src/ipc/ConnectEvent.h
        src/ipc/DisconnectEvent.cpp
        src/ipc/DisconnectEvent.h
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
        src/TimeoutException.cpp
        src/TimeoutException.h
        src/logging/easylogging++.cpp
        src/logging/easylogging++.h
        src/ConfigParser.cpp
        src/ConfigParser.h
        src/metrics/MetricsManager.cpp
        src/metrics/MetricsManager.h
        src/logging/loginit.h
        src/logging/loginit.h
        src/Configuration.cpp
        src/Configuration.h)
target_link_libraries(wimoved nl-3 nl-route-3 prometheus-cpp::pull prometheus-cpp::core)

include_directories(src/hostapd)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
