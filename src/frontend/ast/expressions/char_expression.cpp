#include <iomanip>
#include <sstream>

#include "ast/ast_writer.hpp"
#include "ast/expressions/char_expression.hpp"

CharExpression::CharExpression(i32 value, Location location)
    : Expression(ExpressionType::literal, location),
      _value(value) {
}

CharExpression::~CharExpression() {}

i32 CharExpression::value() const noexcept {
    return _value;
}

void CharExpression::write(AstWriter* const writer) {
    std::ostringstream oss;
    oss << "U+" << std::hex << value();

    writer->write(oss.str());
}
