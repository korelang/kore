#include "ast/ast_writer.hpp"
#include "ast/expressions/float_expression.hpp"

namespace kore {
    FloatExpression::FloatExpression(f32 value, SourceLocation location)
        : Expression(ExpressionType::Literal, location),
        _value(value) {
    }

    FloatExpression::~FloatExpression() {
    }

    const Type* FloatExpression::type() const {
        return &_type;
    }

    f32 FloatExpression::value() const noexcept {
        return _value;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(FloatExpression)
}
