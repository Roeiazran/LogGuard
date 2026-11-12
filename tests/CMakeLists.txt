add_executable(logguard_tests
    unit/core/VersionTest.cpp
)

target_link_libraries(logguard_tests
    PRIVATE
        logguard::core
        GTest::gtest_main
)

target_compile_definitions(logguard_tests
    PRIVATE
        LOGGUARD_EXPECTED_VERSION="${PROJECT_VERSION}"
)

logguard_set_warnings(logguard_tests)

include(GoogleTest)
gtest_discover_tests(logguard_tests)
