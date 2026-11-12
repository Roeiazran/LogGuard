# Strict compiler warnings, applied only to LogGuard's own targets (never globally,
# so third-party code is not affected). Usage: logguard_set_warnings(<target>)

option(LOGGUARD_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

function(logguard_set_warnings target)
    set(common_warnings
        -Wall                   # the common, high-value warnings
        -Wextra                 # more useful warnings not in -Wall
        -Wpedantic              # non-standard C++ extensions
        -Wshadow                # a variable hides another with the same name
        -Wconversion            # implicit conversions that may lose data
        -Wsign-conversion       # implicit signed <-> unsigned conversions
        -Wold-style-cast        # C-style casts instead of static_cast etc.
        -Wnon-virtual-dtor      # class with virtual functions but non-virtual destructor
        -Woverloaded-virtual    # a function hides a virtual function of the base class
        -Wcast-align            # casts that increase required alignment
        -Wnull-dereference      # possible null pointer dereference
        -Wdouble-promotion      # float silently promoted to double
        -Wformat=2              # unsafe printf-style format strings
        -Wimplicit-fallthrough  # switch case falls through without [[fallthrough]]
    )

    set(gcc_only_warnings
        -Wmisleading-indentation  # indentation suggests a block that isn't one
        -Wduplicated-cond         # same condition in an if / else-if chain
        -Wduplicated-branches     # if and else branches do the same thing
        -Wlogical-op              # suspicious use of && / ||
        -Wuseless-cast            # cast to the type the value already has
    )

    set(msvc_warnings
        /W4                     # high warning level
        /permissive-            # strict standard conformance
    )

    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:${common_warnings}>
        $<$<CXX_COMPILER_ID:GNU>:${gcc_only_warnings}>
        $<$<CXX_COMPILER_ID:MSVC>:${msvc_warnings}>
    )

    set_target_properties(${target} PROPERTIES
        COMPILE_WARNING_AS_ERROR ${LOGGUARD_WARNINGS_AS_ERRORS}
    )
endfunction()
