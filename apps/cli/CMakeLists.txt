add_executable(logguard
    main.cpp
)

target_link_libraries(logguard
    PRIVATE
        logguard::core
)

logguard_set_warnings(logguard)
