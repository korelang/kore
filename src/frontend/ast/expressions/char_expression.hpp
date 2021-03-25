#ifndef KORE_CHAR_EXPRESSION_HPP
#define KORE_CHAR_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"

/// An char literal
class CharExpression : public Expression {
    public:
        CharExpression(i32 value, Location location);
        virtual ~CharExpression();

        i32 value() const noexcept;

        void write(AstWriter* const writer) override;

    private:
        i32 _value;
};

#endif // KORE_CHAR_EXPRESSION_HPP
