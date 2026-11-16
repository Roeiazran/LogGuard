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
