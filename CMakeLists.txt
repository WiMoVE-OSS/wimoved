cmake_minimum_required(VERSION 3.13)

project(wimoved LANGUAGES CXX)

find_package(prometheus-cpp CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LIBRARIES nl-3 nl-route-3 prometheus-cpp::pull prometheus-cpp::core)

add_compile_definitions(ELPP_THREAD_SAFE)
add_compile_definitions(ELPP_FEATURE_CRASH_LOG)

file(GLOB_RECURSE SRC_FILES src/*.h src/*.cpp src/**/*.h src/**/*.cpp)
file(GLOB_RECURSE TEST_FILES test/*.h test/*.cpp test/**/*.h test/**/*.cpp)
file(GLOB_RECURSE VENDOR_FILES vendor/*.h vendor/*.cpp vendor/**/*.h vendor/**/*.cpp)

add_executable(wimoved ${SRC_FILES} ${VENDOR_FILES})
if(DEFINED STAGING_DIR)
    target_include_directories(wimoved PUBLIC vendor ${STAGING_DIR}/usr/include/libnl3)
    add_compile_definitions(ELPP_SYSLOG)
else()
    target_include_directories(wimoved PUBLIC vendor /usr/include/libnl3)
endif()
target_link_libraries(wimoved ${LIBRARIES})

add_custom_target(format COMMAND clang-format -i ${SRC_FILES})
add_custom_target(format-check COMMAND clang-format --dry-run --Werror ${SRC_FILES})
add_custom_target(lint COMMAND run-clang-tidy -j8 -quiet ${SRC_FILES})
add_custom_target(precommit DEPENDS lint format)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
list(FILTER SRC_FILES EXCLUDE REGEX .*main\\.cpp$)

add_executable(wimoved_tests ${SRC_FILES} ${VENDOR_FILES} ${TEST_FILES})
target_link_libraries(wimoved_tests gtest_main ${LIBRARIES})
target_include_directories(wimoved_tests PUBLIC src test include vendor /usr/include/libnl3)
gtest_discover_tests(wimoved_tests)