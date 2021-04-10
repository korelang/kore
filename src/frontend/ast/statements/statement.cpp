#include "ast/expressions/expression.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/statements/function.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/statements/module_statement.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"

Statement::Statement() : AstNode(Location::unknown) {}

Statement::Statement(Location location) : AstNode(location) {}

Statement::~Statement() {}

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

Statement* Statement::make_variable_assignment(const Token& identifier, const Token& type, Expression* expr) {
    return new VariableAssignment(identifier, type, expr);
}
