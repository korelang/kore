#ifndef KORE_STRING_EXPRESSION_HPP
#define KORE_STRING_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"

/// A string literal
class StringExpression : public Expression {
    public:
        StringExpression(const std::string& value, Location location);
        virtual ~StringExpression();

        std::string value() const noexcept;

        void write(AstWriter* const writer) override;

    private:
        std::string _value;
};

#endif // KORE_STRING_EXPRESSION_HPP
