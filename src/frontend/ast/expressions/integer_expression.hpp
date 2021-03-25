#ifndef KORE_INTEGER_EXPRESSION_HPP
#define KORE_INTEGER_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"

/// An integer literal
class IntegerExpression : public Expression {
    public:
        IntegerExpression(i32 value, Location location);
        virtual ~IntegerExpression();

        i32 value() const noexcept;

        void write(AstWriter* const writer) override;

    private:
        i32 _value;
};

#endif // KORE_INTEGER_EXPRESSION_HPP
