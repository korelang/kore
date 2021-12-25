#ifndef BRANCH_HPP
#define BRANCH_HPP

#include "ast/statements/statement_list.hpp"

class Branch : public Statement {
    public:
        Branch(Expression* condition, StatementList statements);
        Branch(StatementList statements);
        virtual ~Branch();

        void write(AstWriter* const writer) override;

    private:
        Expression* _condition;
        StatementList _statements;
};

#endif // BRANCH_HPP
