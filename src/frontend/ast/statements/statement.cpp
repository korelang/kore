#include "ast/expressions/expression.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/statements/module_statement.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "ast/statements/variable_assignment.hpp"

Statement::Statement() : AstNode(Location::unknown) {}

Statement::Statement(Location location) : AstNode(location) {}

Statement::~Statement() {}

Statement* Statement::make_module_decl(const std::string& module_name, const Location& location) {
    return new ModuleStatement(module_name, location);
}

Statement* Statement::make_import_decl(const std::string& import_spec, const Location& location) {
    return new ImportStatement(import_spec, location);
}

Statement* Statement::make_variable_decl(const Token& identifier, const Token& type) {
    return new VariableDeclaration(identifier, type);
}

Statement* Statement::make_variable_assignment(const Token& identifier, const Token& type, Expression* expr) {
    return new VariableAssignment(identifier, type, expr);
}
