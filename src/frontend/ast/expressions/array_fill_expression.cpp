#include "ast/ast_writer.hpp"
#include "ast/expressions/array_fill_expression.hpp"

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

void ArrayFillExpression::write(AstWriter* const writer) {
    writer->write("array_fill<");
    _size_expr->write(writer);
    writer->write(":");
    _element_expr->write(writer);
    writer->write(">");
}
