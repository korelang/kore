#include <iomanip>
#include <sstream>

#include "ast/ast_writer.hpp"
#include "ast/expressions/char_expression.hpp"

namespace kore {
    CharExpression::CharExpression(i32 value, Location location)
        : Expression(ExpressionType::Literal, location),
        _value(value) {
    }

    CharExpression::~CharExpression() {}

    i32 CharExpression::value() const noexcept {
        return _value;
    }

    const Type* CharExpression::type() const {
        return &_type;
    }

    void CharExpression::write(AstWriter* const writer) {
        std::ostringstream oss;
        oss << "U+" << std::hex << value();

        writer->write(oss.str());
    }
}
