#ifndef KORE_RETURN_STATEMENT_HPP
#define KORE_RETURN_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "ast/expressions/expression.hpp"

class Return : public Statement {
    public:
        Return(Expression* expr);
        virtual ~Return();

        void write(AstWriter* const writer) override;

    private:
        Expression::pointer _expr;
};

#endif // KORE_RETURN_STATEMENT_HPP
