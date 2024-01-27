#ifndef KORE_INDEX_EXPRESSION_HPP
#define KORE_INDEX_EXPRESSION_HPP

#include <vector>

#include "ast/expressions/expression.hpp"

namespace kore {
    class ArrayType;

    /// An arbitrary index expression (array index, map index etc.)
    class IndexExpression : public Expression {
        public:
            IndexExpression();
            IndexExpression(
                Owned<Expression> array_expr,
                Owned<Expression> index_expr,
                const SourceLocation& location
            );
            virtual ~IndexExpression();

            Expression* expr();
            Expression* index_expr();
            const Type* type() const override;

            KORE_AST_VISITOR_ACCEPT_METHOD_VALUE_CONTEXT_DEFINITION

        private:
            Owned<Expression> _array_expr;
            Owned<Expression> _index_expr;
    };
}

#endif // KORE_INDEX_EXPRESSION_HPP
