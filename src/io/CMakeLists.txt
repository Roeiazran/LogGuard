add_library(logguard_io
    LineReader.cpp
)
add_library(logguard::io ALIAS logguard_io)

target_link_libraries(logguard_io
    PUBLIC
        logguard::core
)

logguard_configure_target(logguard_io)
