#ifndef KORE_FLOAT_EXPRESSION_HPP
#define KORE_FLOAT_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"

/// A float literal
class FloatExpression : public Expression {
    public:
        FloatExpression(f32 value, Location location);
        virtual ~FloatExpression();

        f32 value() const noexcept;

        void write(AstWriter* const writer) override;

    private:
        f32 _value;
};

#endif // KORE_FLOAT_EXPRESSION_HPP
