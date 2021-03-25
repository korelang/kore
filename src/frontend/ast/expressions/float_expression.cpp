#include "ast/ast_writer.hpp"
#include "ast/expressions/float_expression.hpp"

FloatExpression::FloatExpression(f32 value, Location location)
    : Expression(ExpressionType::literal, location),
      _value(value) {
}

FloatExpression::~FloatExpression() {}

f32 FloatExpression::value() const noexcept {
    return _value;
}

void FloatExpression::write(AstWriter* const writer) {
    writer->write(std::to_string(value()));
}
