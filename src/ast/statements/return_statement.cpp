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

    const Expression* Return::expr() const {
        return _expr.get();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(Return)
}
