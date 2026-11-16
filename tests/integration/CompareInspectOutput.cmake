# Runs `logguard inspect` on INPUT and compares its output with EXPECTED.
# Called by CTest as: cmake -DLOGGUARD=... -DINPUT=... -DEXPECTED=... -DOUTPUT=... -P <this file>

execute_process(
    COMMAND "${LOGGUARD}" inspect --input "${INPUT}" --format bgl
    OUTPUT_FILE "${OUTPUT}"
    RESULT_VARIABLE exit_code
)
if(NOT exit_code EQUAL 0)
    message(FATAL_ERROR "logguard inspect exited with code ${exit_code}")
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} -E compare_files "${OUTPUT}" "${EXPECTED}"
    RESULT_VARIABLE differs
)
if(differs)
    message(FATAL_ERROR "Output differs from expected.\n  actual:   ${OUTPUT}\n  expected: ${EXPECTED}")
endif()
