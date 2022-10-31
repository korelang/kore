set(TEST_SCANNER_RUNNER_NAME "test_scanner")

set(TEST_RUNNER_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/tests"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/frontend"
    # "${PROJECT_SOURCE_DIR}/src/backend"
)

set(TEST_RUNNER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast_node.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/array_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/array_fill_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/array_range_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/binary_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/bool_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/call.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/char_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/float_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/identifier.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/integer_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/parameter.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/string_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/unary_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/parser_error_node.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/expression_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/function.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/import_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/module_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/return_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/variable_assignment.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/variable_declaration.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/keywords.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/location.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/operator.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/string-utils.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/token.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/token_category.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/token_type.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/scanner.cpp
    ${PROJECT_SOURCE_DIR}/src/errors/error.cpp
    ${PROJECT_SOURCE_DIR}/src/errors/errors.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/color_attributes.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/colors.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/logging.cpp
    ${PROJECT_SOURCE_DIR}/src/utf8/utf8.cpp

    # Types
    ${PROJECT_SOURCE_DIR}/src/types/array_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/bool_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/char_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/float_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/function_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/integer_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/str_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/unknown_type.cpp

    # Utils
    ${PROJECT_SOURCE_DIR}/src/utils/endian.cpp

    # Files specific to scanner test runner
    test_utils.cpp
    scanner/test_literals.cpp
    main.cpp
)

add_executable(${TEST_SCANNER_RUNNER_NAME} ${TEST_RUNNER_SOURCES})

target_include_directories(${TEST_SCANNER_RUNNER_NAME} PRIVATE ${TEST_RUNNER_INCLUDE_DIRS})

target_compile_options(${TEST_SCANNER_RUNNER_NAME} PUBLIC ${SHARED_COMPILE_FLAGS})

set_target_properties(${TEST_SCANNER_RUNNER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests/bin") 
