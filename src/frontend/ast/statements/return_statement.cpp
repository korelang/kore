#include "ast/ast_visitor.hpp"
#include "ast/statements/return_statement.hpp"

namespace kore {
    Return::Return() : _expr(nullptr) {}

    Return::Return(Owned<Expression> expr)
        : Statement(expr->location(), StatementType::Return),
          _expr(std::move(expr)) {}

    Return::~Return() {}

    Expression* Return::expr() {
        return _expr.get();
    }

    void Return::accept(AstVisitor& visitor) {
        if (_expr) {
            _expr->accept(visitor);
        }

        visitor.visit(*this);
    }

    void Return::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
