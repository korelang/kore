#include "ast/ast_writer.hpp"
#include "ast/expressions/array_range_expression.hpp"

namespace kore {
    ArrayRangeExpression::ArrayRangeExpression(
        Owned<Expression> start_expr,
        Owned<Expression> end_expr,
        const SourceLocation& location
    ) : Expression(ExpressionType::Array, location),
        _start_expr(std::move(start_expr)),
        _end_expr(std::move(end_expr)) {
    }

    ArrayRangeExpression::~ArrayRangeExpression() {}

    bool ArrayRangeExpression::uses_constants_only() const {
        return false;
    }

    Expression* ArrayRangeExpression::start_expr() {
        return _start_expr.get();
    }

    Expression* ArrayRangeExpression::end_expr() {
        return _end_expr.get();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(ArrayRangeExpression)
}
