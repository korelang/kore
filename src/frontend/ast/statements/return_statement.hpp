#ifndef KORE_RETURN_STATEMENT_HPP
#define KORE_RETURN_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    class Return : public Statement {
        public:
            Return();
            Return(Expression* expr);
            virtual ~Return();

            Expression* expr();

            void write(AstWriter* const writer) override;
            void accept(AstVisitor* visitor) override;

        private:
            Expression::pointer _expr;
    };
}

#endif // KORE_RETURN_STATEMENT_HPP
