#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/integer_expression.hpp"

namespace kore {
    IntegerExpression::IntegerExpression(i32 value, Location location)
        : Expression(ExpressionType::Literal, location),
        _value(value),
        _type(32) {
    }

    IntegerExpression::~IntegerExpression() {}

    i32 IntegerExpression::value() const noexcept {
        return _value;
    }

    const Type* IntegerExpression::type() const {
        return static_cast<const Type*>(&_type);
    }

    void IntegerExpression::accept(AstVisitor& visitor) {
        accept_visit_only(visitor);
    }

    void IntegerExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
