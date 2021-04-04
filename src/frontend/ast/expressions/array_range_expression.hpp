#ifndef KORE_ARRAY_RANGE_EXPRESSION_HPP
#define KORE_ARRAY_RANGE_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

class ArrayRangeExpression : public Expression {
    public:
        ArrayRangeExpression(Expression* start_expr, Expression* end_expr, const Location& location);
        virtual ~ArrayRangeExpression();

        bool uses_constants_only() const;

        void write(AstWriter* const writer) override;

    private:
        pointer _start_expr, _end_expr;
};

#endif // KORE_ARRAY_RANGE_EXPRESSION_HPP
