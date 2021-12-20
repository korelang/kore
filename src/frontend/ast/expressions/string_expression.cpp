#include "ast/ast_writer.hpp"
#include "ast/expressions/string_expression.hpp"

StringExpression::StringExpression(const std::string& value, Location location)
    : Expression(ExpressionType::Literal, location),
      _value(value) {
}

StringExpression::~StringExpression() {}

std::string StringExpression::value() const noexcept {
    return _value;
}

const Type* StringExpression::type() const {
    return static_cast<const Type*>(&_type);
}

void StringExpression::write(AstWriter* const writer) {
    writer->write(value());
}
