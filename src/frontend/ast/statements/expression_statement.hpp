#ifndef KORE_EXPRESSION_STATEMENT_HPP
#define KORE_EXPRESSION_STATEMENT_HPP

#include <memory>

#include "ast/statements/statement.hpp"

namespace kore {
    /// Statement that wraps an expression. Useful for
    /// AST nodes that can both be expressions and statements
    /// such as function calls or if statements
    class ExpressionStatement final : public Statement {
        public:
            ExpressionStatement(Expression* expression);
            virtual ~ExpressionStatement();

            Expression* expression();

            void write(AstWriter* const writer) override;
            void accept(AstVisitor* visitor) override;

        private:
            std::unique_ptr<Expression> _expression;
    };
}

#endif // KORE_EXPRESSION_STATEMENT_HPP
