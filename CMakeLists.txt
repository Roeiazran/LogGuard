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

add_subdirectory(src)
