#include <exception>

#include "ast/expressions/expression.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/statements/function.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/statements/module_statement.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "ast/statements/expression_statement.hpp"
#include "types/type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Statement::Statement() : AstNode(SourceLocation::unknown) {}

    Statement::Statement(SourceLocation location, StatementType statement_type)
        : AstNode(location), _statement_type(statement_type) {}

    Statement::~Statement() {}

    void Statement::add_statement(Owned<Statement> statement) {
        UNUSED_PARAM(statement);

        throw std::runtime_error("This statement type does not contain other statements");
    }

    StatementType Statement::statement_type() const {
        return _statement_type;
    }

    Function* Statement::make_function(bool exported, const Token& func_name) {
        return new Function(exported, func_name);
    }

    Statement* Statement::make_module_decl(const std::string& module_name, const SourceLocation& location) {
        return new ModuleStatement(module_name, location);
    }

    Statement* Statement::make_module_decl(const Token& token) {
        return new ModuleStatement(token);
    }

    Statement* Statement::make_variable_decl(const Token& identifier, const Token& type) {
        return new VariableDeclaration(identifier, type);
    }
}
