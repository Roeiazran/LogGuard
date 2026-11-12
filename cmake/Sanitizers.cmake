# Runtime error detectors ("sanitizers") for LogGuard's own targets.
#   address   (ASan)  - buffer overflows, use-after-free, double free
#   undefined (UBSan) - undefined behavior: signed overflow, bad shifts, misaligned access...
#   thread    (TSan)  - data races between threads
# Usage: configure with -DLOGGUARD_SANITIZERS="address;undefined" (or use the asan/tsan presets),
#        then call logguard_enable_sanitizers(<target>) for each target.

set(LOGGUARD_SANITIZERS "" CACHE STRING
    "Sanitizers to enable: semicolon-separated subset of address;undefined;thread")

set(_logguard_known_sanitizers address undefined thread)
foreach(sanitizer IN LISTS LOGGUARD_SANITIZERS)
    if(NOT sanitizer IN_LIST _logguard_known_sanitizers)
        message(FATAL_ERROR "Unknown sanitizer '${sanitizer}'. Allowed: ${_logguard_known_sanitizers}")
    endif()
endforeach()

if("thread" IN_LIST LOGGUARD_SANITIZERS AND "address" IN_LIST LOGGUARD_SANITIZERS)
    message(FATAL_ERROR "ThreadSanitizer cannot be combined with AddressSanitizer. Use separate builds.")
endif()

if(LOGGUARD_SANITIZERS AND MSVC)
    message(FATAL_ERROR "LOGGUARD_SANITIZERS is only supported with GCC and Clang.")
endif()

function(logguard_enable_sanitizers target)
    if(NOT LOGGUARD_SANITIZERS)
        return()
    endif()

    list(JOIN LOGGUARD_SANITIZERS "," sanitizer_list)
    set(sanitizer_flags
        -fsanitize=${sanitizer_list}
        -fno-omit-frame-pointer          # readable stack traces in reports
    )
    if("undefined" IN_LIST LOGGUARD_SANITIZERS)
        list(APPEND sanitizer_flags -fno-sanitize-recover=undefined)  # UB fails the test
    endif()

    target_compile_options(${target} PRIVATE ${sanitizer_flags})
    target_link_options(${target} PRIVATE ${sanitizer_flags})
endfunction()
