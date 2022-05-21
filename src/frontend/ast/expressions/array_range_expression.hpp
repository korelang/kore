#ifndef KORE_ARRAY_RANGE_EXPRESSION_HPP
#define KORE_ARRAY_RANGE_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class ArrayRangeExpression : public Expression {
        public:
            ArrayRangeExpression(Expression* start_expr, Expression* end_expr, const Location& location);
            virtual ~ArrayRangeExpression();

            bool uses_constants_only() const;
            Expression* start_expr();
            Expression* end_expr();

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            pointer _start_expr, _end_expr;
    };
}

#endif // KORE_ARRAY_RANGE_EXPRESSION_HPP
