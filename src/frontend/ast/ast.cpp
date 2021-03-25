#include "ast.hpp"

Ast::Ast() {}

Ast::Ast(Ast&& ast)
    : _module_name(ast._module_name),
      _root(std::move(ast._root)),
      _statements(std::move(ast._statements)) {
}

Ast::~Ast() {
}

std::string Ast::module_name() const {
    return _module_name;
}

bool Ast::is_main() const noexcept {
    return _module_name == "main";
}

void Ast::add_statement(Statement* statement) {
    _statements.emplace_back(statement);
}

void Ast::write(AstWriter* const writer) const {
    for (const auto& statement : _statements) {
        statement->write(writer);
    }
}
