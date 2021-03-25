#include "ast/ast_writer.hpp"
#include "ast/expressions/string_expression.hpp"

StringExpression::StringExpression(const std::string& value, Location location)
    : Expression(ExpressionType::literal, location),
      _value(value) {
}

StringExpression::~StringExpression() {}

std::string StringExpression::value() const noexcept {
    return _value;
}

void StringExpression::write(AstWriter* const writer) {
    writer->write(value());
}
