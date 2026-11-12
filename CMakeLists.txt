cmake_minimum_required(VERSION 3.25)

project(LogGuard
    VERSION 0.1.0
    DESCRIPTION "C++20 log anomaly detection engine"
    LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed. Use: cmake -S . -B build/debug")
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

option(LOGGUARD_BUILD_TESTS "Build the LogGuard test suite" ${PROJECT_IS_TOP_LEVEL})

include(cmake/Dependencies.cmake)

add_subdirectory(src)
add_subdirectory(apps/cli)

if(LOGGUARD_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
