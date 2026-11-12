# Static analysis with clang-tidy, run automatically while compiling LogGuard's own targets.
# Checks are configured in the repository's .clang-tidy file.
# Usage: configure with -DLOGGUARD_ENABLE_CLANG_TIDY=ON (or use the lint preset),
#        then call logguard_enable_clang_tidy(<target>) for each target.

option(LOGGUARD_ENABLE_CLANG_TIDY "Run clang-tidy while compiling LogGuard targets" OFF)

if(LOGGUARD_ENABLE_CLANG_TIDY)
    find_program(LOGGUARD_CLANG_TIDY_EXE
        NAMES clang-tidy clang-tidy-20 clang-tidy-19 clang-tidy-18
        HINTS /opt/homebrew/opt/llvm/bin /usr/local/opt/llvm/bin
        REQUIRED
    )
    message(STATUS "clang-tidy: ${LOGGUARD_CLANG_TIDY_EXE}")

    set(_logguard_clang_tidy_command "${LOGGUARD_CLANG_TIDY_EXE}")

    # Homebrew's clang-tidy does not know where Apple's SDK headers live.
    if(APPLE)
        execute_process(
            COMMAND xcrun --show-sdk-path
            OUTPUT_VARIABLE _logguard_macos_sdk
            OUTPUT_STRIP_TRAILING_WHITESPACE
            COMMAND_ERROR_IS_FATAL ANY
        )
        list(APPEND _logguard_clang_tidy_command "--extra-arg=-isysroot${_logguard_macos_sdk}")
    endif()

    if(LOGGUARD_WARNINGS_AS_ERRORS)
        list(APPEND _logguard_clang_tidy_command "--warnings-as-errors=*")
    endif()
endif()

function(logguard_enable_clang_tidy target)
    if(LOGGUARD_ENABLE_CLANG_TIDY)
        set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${_logguard_clang_tidy_command}")
    endif()
endfunction()
