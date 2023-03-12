#ifndef KORE_EXPRESSION_STATEMENT_HPP
#define KORE_EXPRESSION_STATEMENT_HPP

#include <memory>

#include "ast/expressions/expression.hpp"
#include "ast/statements/statement.hpp"

namespace kore {
    /// Statement that wraps an expression. Useful for
    /// AST nodes that can both be expressions and statements
    /// such as function calls or if statements
    class ExpressionStatement final : public Statement {
        public:
            ExpressionStatement(Owned<Expression> expression);
            virtual ~ExpressionStatement();

            Expression* expression();

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            Expression::pointer _expression;
    };
}

#endif // KORE_EXPRESSION_STATEMENT_HPP
