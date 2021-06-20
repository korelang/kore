cmake_minimum_required(VERSION 3.8)

project(kore)
set(CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIRS
    "./src"
    "./src/frontend"
)

set(BASE_SOURCES
    src/frontend/ast/ast.cpp
    src/frontend/ast/ast_node.cpp
    src/frontend/ast/ast_writer.cpp
    src/frontend/ast/ast_stream_writer.cpp

    # Expressions
    src/frontend/ast/parser_error_node.cpp
    src/frontend/ast/expressions/array_fill_expression.cpp
    src/frontend/ast/expressions/array_expression.cpp
    src/frontend/ast/expressions/array_range_expression.cpp
    src/frontend/ast/expressions/binary_expression.cpp
    src/frontend/ast/expressions/bool_expression.cpp
    src/frontend/ast/expressions/char_expression.cpp
    src/frontend/ast/expressions/expression.cpp
    src/frontend/ast/expressions/float_expression.cpp
    src/frontend/ast/expressions/identifier.cpp
    src/frontend/ast/expressions/integer_expression.cpp
    src/frontend/ast/expressions/string_expression.cpp
    src/frontend/ast/expressions/unary_expression.cpp

    # Statements
    src/frontend/ast/statements/statement.cpp
    src/frontend/ast/statements/branch.cpp
    src/frontend/ast/statements/if_statement.cpp
    src/frontend/ast/statements/function.cpp
    src/frontend/ast/statements/return_statement.cpp
    src/frontend/ast/statements/import_statement.cpp
    src/frontend/ast/statements/module_statement.cpp
    src/frontend/ast/statements/variable_declaration.cpp
    src/frontend/ast/statements/variable_assignment.cpp

    src/frontend/errors.cpp
    src/frontend/keywords.cpp
    src/frontend/location.cpp
    src/frontend/operator.cpp
    src/frontend/parser.cpp
    src/frontend/scanner.cpp
    src/frontend/string-utils.cpp
    src/frontend/token.cpp
    src/frontend/token_category.cpp
    src/frontend/token_type.cpp

    # Types
    src/types/type.cpp
    src/types/array_type.cpp
    src/types/bool_type.cpp
    src/types/char_type.cpp
    src/types/integer_type.cpp
    src/types/optional.cpp
    src/types/str_type.cpp
    src/types/unknown_type.cpp

    src/logging/color_attributes.cpp
    src/logging/colors.cpp
    src/logging/logging.cpp
    src/utf8/utf8.cpp
    src/options.cpp
)

set(SOURCES
    ${BASE_SOURCES}
    src/main.cpp
)

set(COMPILER_NAME "korec")

set(SHARED_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror)

# Main compiler executable
add_executable(${COMPILER_NAME} ${SOURCES})

add_definitions("-DNDEBUG -g3 -O1")

include_directories(${COMPILER_NAME} ${INCLUDE_DIRS})

target_compile_options(${COMPILER_NAME} PUBLIC ${SHARED_COMPILE_FLAGS})

set_target_properties(${COMPILER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") 

# Test runner executable
set(TEST_RUNNER_NAME "test_runner")

set(TEST_SOURCES
    ${BASE_SOURCES}
    tests/main.cpp
    tests/scanner/test_literals.cpp
)

add_executable(${TEST_RUNNER_NAME} ${TEST_SOURCES})

include_directories(${TEST_RUNNER_NAME} "./tests" ${INCLUDE_DIRS})

target_compile_options(${TEST_RUNNER_NAME} PUBLIC ${SHARED_COMPILE_FLAGS})

set_target_properties(${TEST_RUNNER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") 
