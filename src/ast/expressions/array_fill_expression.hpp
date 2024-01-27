#ifndef KORE_ARRAY_FILL_EXPRESSION_HPP
#define KORE_ARRAY_FILL_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class ArrayFillExpression : public Expression {
        public:
            ArrayFillExpression(
                Owned<Expression> size_expr,
                Owned<Expression> element_expr,
                const SourceLocation& location
            );
            virtual ~ArrayFillExpression();

            bool uses_constants_only() const;
            Expression* size_expr();
            Expression* expr();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            pointer _size_expr;
            pointer _element_expr;
    };
}

#endif // KORE_ARRAY_FILL_EXPRESSION_HPP
