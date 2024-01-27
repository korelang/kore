#ifndef KORE_ARRAY_RANGE_EXPRESSION_HPP
#define KORE_ARRAY_RANGE_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class ArrayRangeExpression : public Expression {
        public:
            ArrayRangeExpression(
                Owned<Expression> start_expr,
                Owned<Expression> end_expr,
                const SourceLocation& location
            );
            virtual ~ArrayRangeExpression();

            bool uses_constants_only() const;
            Expression* start_expr();
            Expression* end_expr();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            pointer _start_expr, _end_expr;
    };
}

#endif // KORE_ARRAY_RANGE_EXPRESSION_HPP
