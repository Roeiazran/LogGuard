add_executable(logguard_tests
    unit/core/VersionTest.cpp
    unit/core/ResultTest.cpp
    unit/core/Utf8Test.cpp
    unit/bgl/BglTimestampTest.cpp
    unit/bgl/BglParserTest.cpp
    unit/bgl/LabeledBglParserTest.cpp
    unit/bgl/InspectStatsTest.cpp
    unit/io/LineReaderTest.cpp
)

target_link_libraries(logguard_tests
    PRIVATE
        # logguard::core comes in automatically: bgl and io link it PUBLIC.
        logguard::bgl
        logguard::io
        GTest::gtest_main
)

target_compile_definitions(logguard_tests
    PRIVATE
        LOGGUARD_EXPECTED_VERSION="${PROJECT_VERSION}"
)

logguard_configure_target(logguard_tests)

include(GoogleTest)
gtest_discover_tests(logguard_tests)

# ---------------------------------------------------------------
# Integration tests: run the real logguard executable.
# ---------------------------------------------------------------

# Output on the checked-in fixture must match the expected file byte for byte.
add_test(NAME cli.inspect.matches_expected_output
    COMMAND ${CMAKE_COMMAND}
        -DLOGGUARD=$<TARGET_FILE:logguard>
        -DINPUT=${PROJECT_SOURCE_DIR}/data/fixtures/bgl-small.log
        -DEXPECTED=${CMAKE_CURRENT_SOURCE_DIR}/integration/inspect-bgl-small.expected.json
        -DOUTPUT=${CMAKE_CURRENT_BINARY_DIR}/inspect-bgl-small.actual.json
        -P ${CMAKE_CURRENT_SOURCE_DIR}/integration/CompareInspectOutput.cmake
)

# A missing input file is a runtime error (exit code 1).
add_test(NAME cli.inspect.missing_file_fails
    COMMAND logguard inspect --input does-not-exist.log --format bgl
)
set_tests_properties(cli.inspect.missing_file_fails PROPERTIES WILL_FAIL TRUE)

# An unknown format is a usage error (exit code 2).
add_test(NAME cli.inspect.unknown_format_fails
    COMMAND logguard inspect --input ${PROJECT_SOURCE_DIR}/data/fixtures/bgl-small.log --format csv
)
set_tests_properties(cli.inspect.unknown_format_fails PROPERTIES WILL_FAIL TRUE)
