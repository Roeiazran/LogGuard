add_library(logguard_core
    core/Version.cpp
)
add_library(logguard::core ALIAS logguard_core)

target_include_directories(logguard_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(logguard_core PUBLIC cxx_std_20)
logguard_configure_target(logguard_core)

# Single source of truth for the version: project(VERSION ...) in the root CMakeLists.txt
target_compile_definitions(logguard_core
    PRIVATE
        LOGGUARD_VERSION="${PROJECT_VERSION}"
)
