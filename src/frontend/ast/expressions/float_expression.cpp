#include "ast/ast_writer.hpp"
#include "ast/expressions/float_expression.hpp"

namespace kore {
    FloatExpression::FloatExpression(f32 value, Location location)
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

    void FloatExpression::write(AstWriter* const writer) {
        writer->write(std::to_string(value()));
    }
}
