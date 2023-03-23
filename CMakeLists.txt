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

file(GLOB_RECURSE ALL_FILES src/*.h src/*.cpp src/**/*.h src/**/*.cpp)

add_executable(wimoved ${ALL_FILES})
target_link_libraries(wimoved nl-3 nl-route-3 prometheus-cpp::pull prometheus-cpp::core)

include_directories(src/hostapd)

add_custom_target(format COMMAND clang-format -i ${ALL_FILES})
add_custom_target(format-check COMMAND clang-format --dry-run --Werror ${ALL_FILES})
add_custom_target(lint COMMAND run-clang-tidy -config-file=.clang-tidy ${ALL_FILES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
