#include "ast/ast_visitor.hpp"
#include "ast/expressions/array_fill_expression.hpp"

namespace kore {
    ArrayFillExpression::ArrayFillExpression(
            Expression* size_expr,
            Expression* element_expr,
            const Location& location
        ) : Expression(ExpressionType::Array, location),
            _size_expr(std::move(size_expr)),
            _element_expr(std::move(element_expr)) {}

    ArrayFillExpression::~ArrayFillExpression() {}

    bool ArrayFillExpression::uses_constants_only() const {
        return false;
        /* return _size_expr->is_literal() && _element_expr->is_literal(); */
    }

    Expression* ArrayFillExpression::size_expr() {
        return _size_expr.get();
    }

    Expression* ArrayFillExpression::expr() {
        return _element_expr.get();
    }

    void ArrayFillExpression::accept(AstVisitor& visitor) {
        _size_expr->accept(visitor);
        _element_expr->accept(visitor);

        visitor.visit(*this);
    }

    void ArrayFillExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
