cmake_minimum_required(VERSION 3.13)

project(gaffa LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
add_definitions(-DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX)
add_executable(gaffa src/hostapd/os_unix.c src/hostapd/wpa_ctrl.c src/IPCSocket.cpp src/main.cpp)

include_directories(src/hostapd)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
