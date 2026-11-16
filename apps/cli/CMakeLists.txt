add_executable(logguard
    main.cpp
    InspectCommand.cpp
)

target_link_libraries(logguard
    PRIVATE
        # logguard::core comes in automatically: bgl and io link it PUBLIC.
        logguard::bgl
        logguard::io
        CLI11::CLI11
        nlohmann_json::nlohmann_json
)

logguard_configure_target(logguard)
