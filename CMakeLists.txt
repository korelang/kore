cmake_minimum_required(VERSION 3.8)

project(kore)

set(CMAKE_CXX_STANDARD 17)
set(KORE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src)

set(KORE_AST_SOURCES
    ${PROJECT_SOURCE_DIR}/src/ast/ast.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/ast_element_stream_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/ast_node.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/ast_stream_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/ast_visitor.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/ast_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/source_location.cpp
)

set(KORE_PARSER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/ast/parser/operator.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/parser/parser.cpp
)

set(KORE_SCANNER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/ast/scanner/keywords.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/scanner/scanner.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/scanner/token.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/scanner/token_category.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/scanner/token_type.cpp
)

set (KORE_AST_EXPRESSION_SOURCES
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/array_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/array_fill_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/array_range_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/binary_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/bool_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/call.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/char_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/float_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/identifier.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/integer_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/parameter.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/string_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/expressions/unary_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/parser_error_node.cpp
)

set(KORE_AST_STATEMENT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/ast/statements/branch.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/expression_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/function.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/if_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/import_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/module_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/return_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/statement.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/variable_assignment.cpp
    ${PROJECT_SOURCE_DIR}/src/ast/statements/variable_declaration.cpp
)

set(KORE_TYPE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/types/array_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/bool_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/char_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/float_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/function_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/integer_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/optional.cpp
    ${PROJECT_SOURCE_DIR}/src/types/scope.cpp
    ${PROJECT_SOURCE_DIR}/src/types/str_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/symbol_table.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type_cache.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type_checker.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type_inferrer.cpp
    ${PROJECT_SOURCE_DIR}/src/types/unknown_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/void_type.cpp
)

set(KORE_ERRORS_SOURCES
    ${PROJECT_SOURCE_DIR}/src/errors/error.cpp
    ${PROJECT_SOURCE_DIR}/src/errors/errors.cpp
)

set(KORE_LOGGING_SOURCES
    ${PROJECT_SOURCE_DIR}/src/logging/color_attributes.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/colors.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/logging.cpp
)

set(KORE_KIR_SOURCES
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/basic_block.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/function.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/graph.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/instruction.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/kir.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/kir_lowering_pass.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/kir/module.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/passes/kir_pass.cpp
)

set(KORE_COMPILER_SOURCES
    ${PROJECT_SOURCE_DIR}/src/compiler/compiler.cpp
    ${PROJECT_SOURCE_DIR}/src/compiler/passes/passes.cpp
)

set(KORE_UTF8_SOURCES
    ${PROJECT_SOURCE_DIR}/src/utf8/utf8.cpp
)

set(KORE_TOP_LEVEL_SOURCES
    ${PROJECT_SOURCE_DIR}/src/version.cpp
)

set(KORE_UTILS_SOURCES
    ${PROJECT_SOURCE_DIR}/src/utils/endian.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/string-utils.cpp
)

set(KORE_TARGETS_BYTECODE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/bytecode_format_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/compiled_object.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/module.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/module_loader.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/bytecode.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/bytecode_array_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/bytecode_codegen.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/codegen/bytecode_codegen2.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/disassemble/decode_instruction.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/disassemble/disassemble.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/disassemble/disassemble_error.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/disassemble/instruction.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/vm/value_type.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/vm/values/array_value.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/bytecode/vm/vm.cpp
)

set(KORE_SHARED_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror)# -g -O0)

# Build main executables:
#   - Bytecode compiler (korec)
#   - Disassembler (koredis)
#   - Virtual machine (kore)
add_subdirectory("./src/bin/korec")
add_subdirectory("./src/bin/koredis")
add_subdirectory("./src/bin/kore")

# Build tests
add_subdirectory("./tests")
