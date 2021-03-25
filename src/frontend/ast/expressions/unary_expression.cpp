#include "ast/ast_writer.hpp"
#include "ast/expressions/unary_expression.hpp"

UnaryExpression::UnaryExpression(
    const std::string& op,
    Expression* expr,
    Location location
) : Expression(ExpressionType::unary, location),
    _op(op),
    _expr(expr)
{}

UnaryExpression::~UnaryExpression() {}

std::string UnaryExpression::op() const {
    return _op;
}

Expression::reference UnaryExpression::expr() {
    return _expr;
}

void UnaryExpression::write(AstWriter* const writer) {
    writer->write(op());
    expr()->write(writer);
}
