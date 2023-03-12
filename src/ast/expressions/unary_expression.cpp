#include "ast/ast_writer.hpp"
#include "ast/expressions/unary_expression.hpp"

namespace kore {
    UnaryExpression::UnaryExpression(
        const std::string& op,
        Expression* expr,
        SourceLocation location
    ) : Expression(ExpressionType::Unary, location),
        _op(op),
        _expr(expr)
    {}

    UnaryExpression::~UnaryExpression() {}

    std::string UnaryExpression::op() const {
        return _op;
    }

    Expression* UnaryExpression::expr() {
        return _expr.get();
    }

    void UnaryExpression::accept(AstVisitor& visitor) {
        _expr->accept(visitor);
        visitor.visit(*this);
    }

    void UnaryExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
