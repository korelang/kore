#include "ast/ast_writer.hpp"
#include "ast/expressions/integer_expression.hpp"

IntegerExpression::IntegerExpression(i32 value, Location location)
    : Expression(ExpressionType::literal, location),
      _value(value) {
}

IntegerExpression::~IntegerExpression() {}

i32 IntegerExpression::value() const noexcept {
    return _value;
}

void IntegerExpression::write(AstWriter* const writer) {
    writer->write(std::to_string(value()));
}
