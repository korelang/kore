#include "ast/ast_visitor.hpp"
#include "ast/statements/module_statement.hpp"

namespace kore {
    ModuleStatement::ModuleStatement(const Token& token)
        : ModuleStatement(token.value(), token.location()) {}

    ModuleStatement::ModuleStatement(const std::string& module_name, const SourceLocation& location)
        : Statement(location, StatementType::ModuleDecl),
        _name(module_name) {
    }

    ModuleStatement::~ModuleStatement() {}

    std::string ModuleStatement::name() const {
        return _name;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(ModuleStatement)
}
