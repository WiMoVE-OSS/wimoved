cmake_minimum_required(VERSION 3.13)

project(wimoved LANGUAGES CXX)

find_package(prometheus-cpp CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
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

file(GLOB_RECURSE SRC_FILES src/*.h src/*.cpp src/**/*.h src/**/*.cpp)
file(GLOB_RECURSE VENDOR_FILES vendor/*.h vendor/*.cpp vendor/**/*.h vendor/**/*.cpp)
list(APPEND ALL_FILES ${SRC_FILES} ${VENDOR_FILES})

add_executable(wimoved ${ALL_FILES} src/MacAddress.cpp src/MacAddress.h)
target_link_libraries(wimoved nl-3 nl-route-3 prometheus-cpp::pull prometheus-cpp::core)

include_directories(vendor)

add_custom_target(format COMMAND clang-format -i ${SRC_FILES})
add_custom_target(format-check COMMAND clang-format --dry-run --Werror ${SRC_FILES})
add_custom_target(lint COMMAND run-clang-tidy -j8 -quiet ${SRC_FILES})
add_custom_target(precommit DEPENDS lint format)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
