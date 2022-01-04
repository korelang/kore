#ifndef KORE_BOOL_EXPRESSION_HPP
#define KORE_BOOL_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "types/bool_type.hpp"

class BoolExpression : public Expression {
    public:
        BoolExpression(const std::string& value, const Location& location);
        virtual ~BoolExpression();

        std::string value() const;
        const Type* type() const override;

        void write(AstWriter* const writer) override;

    private:
        std::string _value;
        BoolType _type;
};

#endif // KORE_BOOL_EXPRESSION_HPP
