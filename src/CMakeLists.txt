add_library(logguard_core
    core/Version.cpp
)
add_library(logguard::core ALIAS logguard_core)

target_include_directories(logguard_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(logguard_core PUBLIC cxx_std_20)
