#ifndef KORE_RETURN_STATEMENT_HPP
#define KORE_RETURN_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    class Return : public Statement {
        public:
            Return();
            Return(Owned<Expression> expr);
            virtual ~Return();

            Expression* expr();
            const Expression* expr() const;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            Expression::pointer _expr;
    };
}

#endif // KORE_RETURN_STATEMENT_HPP
