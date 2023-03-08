set(VM_NAME "kore")

set(VM_INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/frontend"
    "${PROJECT_SOURCE_DIR}/src/backend"
)

set(VM_SOURCES
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast_node.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast_stream_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast_element_stream_writer.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/ast_visitor.cpp

    ${PROJECT_SOURCE_DIR}/src/frontend/keywords.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/source_location.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/operator.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/parser.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/scanner.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/string-utils.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/token.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/token_category.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/token_type.cpp

    # Frontend
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/parser_error_node.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/array_fill_expression.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/expressions/array_expression.cpp
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

    # Statements
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/branch.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/if_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/function.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/expression_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/import_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/module_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/variable_declaration.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/return_statement.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/variable_assignment.cpp
    ${PROJECT_SOURCE_DIR}/src/frontend/ast/statements/expression_statement.cpp

    # Frontend utilities
    ${PROJECT_SOURCE_DIR}/src/logging/color_attributes.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/colors.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/logging.cpp
    ${PROJECT_SOURCE_DIR}/src/utf8/utf8.cpp
    ${PROJECT_SOURCE_DIR}/src/version.cpp

    # Types
    ${PROJECT_SOURCE_DIR}/src/types/type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/array_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/bool_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/char_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/float_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/function_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/integer_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/optional.cpp
    ${PROJECT_SOURCE_DIR}/src/types/str_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/unknown_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type_inferrer.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type_checker.cpp
    ${PROJECT_SOURCE_DIR}/src/types/scope.cpp
    ${PROJECT_SOURCE_DIR}/src/types/symbol_table.cpp
    ${PROJECT_SOURCE_DIR}/src/types/void_type.cpp
    ${PROJECT_SOURCE_DIR}/src/types/type_cache.cpp

    # Errors
    ${PROJECT_SOURCE_DIR}/src/errors/error.cpp
    ${PROJECT_SOURCE_DIR}/src/errors/errors.cpp

    # Frontend utilities
    ${PROJECT_SOURCE_DIR}/src/logging/color_attributes.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/colors.cpp
    ${PROJECT_SOURCE_DIR}/src/logging/logging.cpp
    ${PROJECT_SOURCE_DIR}/src/utf8/utf8.cpp
    ${PROJECT_SOURCE_DIR}/src/version.cpp

    # Backend
    ${PROJECT_SOURCE_DIR}/src/koredis/disassemble.cpp
    ${PROJECT_SOURCE_DIR}/src/koredis/disassemble_error.cpp
    ${PROJECT_SOURCE_DIR}/src/backend/codegen/bytecode/bytecode.cpp
    ${PROJECT_SOURCE_DIR}/src/backend/codegen/compiled_object.cpp
    ${PROJECT_SOURCE_DIR}/src/backend/module.cpp
    ${PROJECT_SOURCE_DIR}/src/backend/module_loader.cpp
    ${PROJECT_SOURCE_DIR}/src/backend/vm/value_type.cpp
    ${PROJECT_SOURCE_DIR}/src/backend/vm/vm.cpp

    # Utils
    ${PROJECT_SOURCE_DIR}/src/utils/endian.cpp

    # Files specific to kore
    repl.cpp
    options.cpp
    main.cpp
)

add_executable(${VM_NAME} ${VM_SOURCES})

target_include_directories(${VM_NAME} PRIVATE ${VM_INCLUDE_DIRS})

target_compile_options(${VM_NAME} PUBLIC ${SHARED_COMPILE_FLAGS})

target_compile_definitions(${VM_NAME} PRIVATE KORE_DEBUG_VM=1)

set_target_properties(${VM_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
