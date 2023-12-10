set(KORE_VM_NAME "kore")

set(KORE_VM_SOURCES
    ${KORE_SCANNER_SOURCES}
    ${KORE_PARSER_SOURCES}
    ${KORE_AST_SOURCES}
    ${KORE_AST_EXPRESSION_SOURCES}
    ${KORE_AST_STATEMENT_SOURCES}
    ${KORE_TYPE_SOURCES}
    ${KORE_LOGGING_SOURCES}
    ${KORE_TOP_LEVEL_SOURCES}
    ${KORE_UTF8_SOURCES}
    ${KORE_UTILS_SOURCES}
    ${KORE_TARGETS_BYTECODE_SOURCES}
    ${KORE_KIR_SOURCES}
    ${KORE_ERRORS_SOURCES}
    ${KORE_ANALYSIS_SOURCES}

    # Files specific to kore
    repl.cpp
    options.cpp
    main.cpp
)

add_definitions("-DNDEBUG")
add_definitions("-g3")
add_definitions("-O1")

add_executable(${KORE_VM_NAME} ${KORE_VM_SOURCES})

include_directories(${KORE_VM_NAME} ${KORE_INCLUDE_DIRS})

target_compile_options(${KORE_VM_NAME} PUBLIC ${KORE_SHARED_COMPILE_FLAGS})

target_compile_definitions(${KORE_VM_NAME} PRIVATE KORE_DEBUG_VM=1)

set_target_properties(
    ${KORE_VM_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)
