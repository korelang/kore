#ifndef KORE_ARRAY_FILL_EXPRESSION_HPP
#define KORE_ARRAY_FILL_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

class ArrayFillExpression : public Expression {
    public:
        ArrayFillExpression(Expression* size_expr, Expression* element_expr, const Location& location);
        virtual ~ArrayFillExpression();

        bool uses_constants_only() const;

        void write(AstWriter* const writer) override;

    private:
        pointer _size_expr;
        pointer _element_expr;
};

#endif // KORE_ARRAY_FILL_EXPRESSION_HPP
