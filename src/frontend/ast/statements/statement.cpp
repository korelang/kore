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
#include "types/type.hpp"

#define UNUSED_PARAM(expr) do { (void)(expr); } while (0)

Statement::Statement() : AstNode(Location::unknown) {}

Statement::Statement(Location location, StatementType statement_type)
    : AstNode(location), _statement_type(statement_type) {}

Statement::~Statement() {}

void Statement::add_statement(Statement* statement) {
    UNUSED_PARAM(statement);

    throw std::runtime_error("This statement type does not contain other statements");
}

StatementType Statement::statement_type() const {
    return _statement_type;
}

Function* Statement::make_function(bool exported, const Token& func_name) {
    return new Function(exported, func_name);
}

Return* Statement::make_return(Expression* expr) {
    return new Return(expr);
}

Statement* Statement::make_module_decl(const std::string& module_name, const Location& location) {
    return new ModuleStatement(module_name, location);
}

Statement* Statement::make_module_decl(const Token& token) {
    return new ModuleStatement(token);
}

Statement* Statement::make_import_decl(Identifier* import_spec) {
    return new ImportStatement(import_spec);
}

Statement* Statement::make_variable_decl(const Token& identifier, const Token& type) {
    return new VariableDeclaration(identifier, type);
}

Statement* Statement::make_variable_assignment(const Token& identifier, Type* type, Expression* expr) {
    return new VariableAssignment(identifier, type, expr);
}
