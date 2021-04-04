#include "ast/ast_writer.hpp"
#include "ast/expressions/array_range_expression.hpp"

ArrayRangeExpression::ArrayRangeExpression(
    Expression* start_expr,
    Expression* end_expr,
    const Location& location
) : Expression(ExpressionType::array, location),
    _start_expr(std::move(start_expr)),
    _end_expr(std::move(end_expr)) {
}

ArrayRangeExpression::~ArrayRangeExpression() {}

bool ArrayRangeExpression::uses_constants_only() const {
    return false;
}

void ArrayRangeExpression::write(AstWriter* const writer) {
    writer->write("array_range<");
    _start_expr->write(writer);
    writer->write(" .. ");
    _end_expr->write(writer);
    writer->write(">");
}
