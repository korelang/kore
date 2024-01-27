#include "ast/ast_visitor.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/expressions/identifier.hpp"

namespace kore {
    ImportStatement::ImportStatement(Owned<Identifier> import_spec)
        : Statement(SourceLocation::unknown, StatementType::ImportDecl),
        _spec(std::move(import_spec)) {
    }

    ImportStatement::~ImportStatement() {}

    std::string ImportStatement::spec() const {
        return _spec->qualified_name();
    }

    Identifier* ImportStatement::identifier() {
        return _spec.get();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(ImportStatement)
}
