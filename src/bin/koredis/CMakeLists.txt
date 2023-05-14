set(KORE_DISASSEMBLER_NAME "koredis")

# TODO: Are all these files necessary? Can we disassociate the includes somehow?
set(KORE_DISASSEMBLER_SOURCES
    ${KORE_AST_SOURCES}
    ${KORE_AST_EXPRESSION_SOURCES}
    ${KORE_AST_STATEMENT_SOURCES}
    ${KORE_SCANNER_SOURCES}
    ${KORE_PARSER_SOURCES}
    ${KORE_TYPE_SOURCES}
    ${KORE_LOGGING_SOURCES}
    ${KORE_TOP_LEVEL_SOURCES}
    ${KORE_UTF8_SOURCES}
    ${KORE_UTILS_SOURCES}
    ${KORE_TARGETS_BYTECODE_SOURCES}
    ${KORE_KIR_SOURCES}
    ${KORE_ERRORS_SOURCES}

    # Files specific to koredis
    # disassemble.cpp
    # disassemble_error.cpp
    # decode_instruction.cpp
    # instruction.cpp
    options.cpp
    output_module.cpp
    main.cpp
)

add_executable(${KORE_DISASSEMBLER_NAME} ${KORE_DISASSEMBLER_SOURCES})

include_directories(${KORE_DISASSEMBLER_NAME} ${KORE_INCLUDE_DIRS})

target_compile_options(${KORE_DISASSEMBLER_NAME} PUBLIC ${KORE_SHARED_COMPILE_FLAGS})

set_target_properties(
    ${KORE_DISASSEMBLER_NAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)
