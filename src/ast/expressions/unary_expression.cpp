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

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(UnaryExpression)
}
