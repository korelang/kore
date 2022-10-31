#include "ast/ast_visitor.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/expressions/identifier.hpp"

namespace kore {
    ImportStatement::ImportStatement(Identifier* import_spec)
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

    void ImportStatement::accept(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    void ImportStatement::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
