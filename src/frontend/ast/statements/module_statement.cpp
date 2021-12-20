#include "ast/ast_writer.hpp"
#include "ast/statements/module_statement.hpp"

ModuleStatement::ModuleStatement(const Token& token)
    : ModuleStatement(token.value(), token.location()) {}

ModuleStatement::ModuleStatement(const std::string& module_name, const Location& location)
    : Statement(location, StatementType::ModuleDecl),
      _name(module_name) {
}

ModuleStatement::~ModuleStatement() {}

std::string ModuleStatement::name() const {
    return _name;
}

void ModuleStatement::write(AstWriter* const writer) {
    writer->write("module ");
    writer->write(name());
    writer->newline();
}
