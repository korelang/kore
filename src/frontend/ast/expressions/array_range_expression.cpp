#include "ast/ast_writer.hpp"
#include "ast/expressions/array_range_expression.hpp"

namespace kore {
    ArrayRangeExpression::ArrayRangeExpression(
        Expression* start_expr,
        Expression* end_expr,
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

    void ArrayRangeExpression::accept(AstVisitor& visitor) {
        _start_expr->accept(visitor);
        _end_expr->accept(visitor);

        visitor.visit(*this);
    }

    void ArrayRangeExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
