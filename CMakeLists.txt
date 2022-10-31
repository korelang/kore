cmake_minimum_required(VERSION 3.8)

project(kore)
set(CMAKE_CXX_STANDARD 14)

set(COMPILER_INCLUDE_DIRS
    "./src"
    "./src/frontend"
    "./src/backend"
)

set(BASE_SOURCES
    src/frontend/ast/ast.cpp
    src/frontend/ast/ast_node.cpp
    src/frontend/ast/ast_writer.cpp
    src/frontend/ast/ast_stream_writer.cpp
    src/frontend/ast/ast_visitor.cpp

    # Expressions
    src/frontend/ast/parser_error_node.cpp
    src/frontend/ast/expressions/array_fill_expression.cpp
    src/frontend/ast/expressions/array_expression.cpp
    src/frontend/ast/expressions/array_range_expression.cpp
    src/frontend/ast/expressions/binary_expression.cpp
    src/frontend/ast/expressions/bool_expression.cpp
    src/frontend/ast/expressions/call.cpp
    src/frontend/ast/expressions/char_expression.cpp
    src/frontend/ast/expressions/expression.cpp
    src/frontend/ast/expressions/float_expression.cpp
    src/frontend/ast/expressions/identifier.cpp
    src/frontend/ast/expressions/integer_expression.cpp
    src/frontend/ast/expressions/parameter.cpp
    src/frontend/ast/expressions/string_expression.cpp
    src/frontend/ast/expressions/unary_expression.cpp

    # Statements
    src/frontend/ast/statements/statement.cpp
    src/frontend/ast/statements/branch.cpp
    src/frontend/ast/statements/if_statement.cpp
    src/frontend/ast/statements/function.cpp
    src/frontend/ast/statements/expression_statement.cpp
    src/frontend/ast/statements/import_statement.cpp
    src/frontend/ast/statements/module_statement.cpp
    src/frontend/ast/statements/variable_declaration.cpp
    src/frontend/ast/statements/return_statement.cpp
    src/frontend/ast/statements/variable_assignment.cpp
    src/frontend/ast/statements/expression_statement.cpp

    src/frontend/keywords.cpp
    src/frontend/source_location.cpp
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
    src/types/float_type.cpp
    src/types/function_type.cpp
    src/types/integer_type.cpp
    src/types/optional.cpp
    src/types/str_type.cpp
    src/types/unknown_type.cpp
    src/types/type_inferrer.cpp
    src/types/type_checker.cpp
    src/types/scope.cpp
    src/types/symbol_table.cpp

    src/errors/error.cpp
    src/errors/errors.cpp
    src/logging/color_attributes.cpp
    src/logging/colors.cpp
    src/logging/logging.cpp
    src/utf8/utf8.cpp
    src/options.cpp
    src/version.cpp

    # Utils
    src/utils/endian.cpp

    # Backend
    src/backend/module.cpp
    src/backend/codegen/bytecode/bytecode.cpp
    src/backend/codegen/bytecode/bytecode_array_writer.cpp
    src/backend/codegen/bytecode/bytecode_codegen.cpp
    src/backend/bytecode_format_writer.cpp
    src/backend/codegen/compiled_object.cpp
    src/backend/vm/value_type.cpp
    src/backend/vm/vm.cpp
    src/backend/decode_instruction.cpp
)

set(COMPILER_SOURCES
    ${BASE_SOURCES}
    src/main.cpp
)

set(COMPILER_NAME "korec")

set(SHARED_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror)

# Main compiler executable
add_executable(${COMPILER_NAME} ${COMPILER_SOURCES})

add_definitions("-DNDEBUG -g3 -O1")

include_directories(${COMPILER_NAME} ${COMPILER_INCLUDE_DIRS})

target_compile_options(${COMPILER_NAME} PUBLIC ${SHARED_COMPILE_FLAGS})

target_compile_definitions(${COMPILER_NAME} PRIVATE
    KORE_DEBUG_TYPECHECKER=1
    KORE_DEBUG_TYPEINFERRER=1
    KORE_DEBUG_BYTECODE_GENERATOR=1
)

set_target_properties(${COMPILER_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Build kore disassembler (koredis)
add_subdirectory("./src/koredis")

# Build tests
add_subdirectory("./tests")
