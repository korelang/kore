set(KORE_VM_NAME "kore")

set(KORE_VM_SOURCES
    ${KORE_AST_SOURCES}
    ${KORE_AST_EXPRESSION_SOURCES}
    ${KORE_AST_STATEMENT_SOURCES}
    ${KORE_TYPE_SOURCES}
    ${KORE_LOGGING_SOURCES}
    ${KORE_TOP_LEVEL_SOURCES}
    ${KORE_UTF8_SOURCES}
    ${KORE_UTILS_SOURCES}
    ${KORE_TARGET_BYTECODE_SOURCES}

    # Files specific to kore
    repl.cpp
    options.cpp
    main.cpp
)

add_executable(${KORE_VM_NAME} ${KORE_VM_SOURCES})

target_compile_options(${KORE_VM_NAME} PUBLIC ${KORE_SHARED_COMPILE_FLAGS})

target_compile_definitions(${KORE_VM_NAME} PRIVATE KORE_DEBUG_VM=1)

set_target_properties(
    ${KORE_VM_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)
