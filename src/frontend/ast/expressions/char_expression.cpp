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

    void CharExpression::accept(AstVisitor& visitor) {
        visitor.visit(*this);
    }

    void CharExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
