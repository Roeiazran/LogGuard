# Third-party dependencies, pinned to exact versions + checksums (see ADR-0005).
# To upgrade: change the version in the URL, then update URL_HASH.

include(FetchContent)

if(LOGGUARD_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        URL      https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
        URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
        SYSTEM
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()
