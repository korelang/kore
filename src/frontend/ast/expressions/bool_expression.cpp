#include "ast/ast_writer.hpp"
#include "ast/expressions/bool_expression.hpp"

BoolExpression::BoolExpression(const std::string& value, const Location& location)
    : Expression(ExpressionType::literal, location),
      _value(value) {
}

BoolExpression::~BoolExpression() {}

std::string BoolExpression::value() const {
    return _value;
}

void BoolExpression::write(AstWriter* const writer) {
    writer->write(_value);
}
