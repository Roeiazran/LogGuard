add_library(logguard_bgl
    ParseError.cpp
    BglTimestamp.cpp
    BglParser.cpp
    InspectStats.cpp
)
add_library(logguard::bgl ALIAS logguard_bgl)

# bgl may use core, never the other way around (see docs/architecture.md §5.1).
target_link_libraries(logguard_bgl
    PUBLIC
        logguard::core
)

logguard_configure_target(logguard_bgl)
