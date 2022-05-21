#include "ast/ast_visitor.hpp"
#include "ast/statements/module_statement.hpp"

namespace kore {
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

    void ModuleStatement::accept(AstVisitor& visitor) {
        accept_visit_only(visitor);
    }

    void ModuleStatement::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
