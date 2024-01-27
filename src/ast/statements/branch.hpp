#ifndef BRANCH_HPP
#define BRANCH_HPP

#include "ast/statements/statement_list.hpp"

namespace kore {
    using body_iterator = std::vector<Statement::pointer>::const_iterator;

    class Branch : public Statement {
        public:
            Branch(Owned<Expression> condition, StatementList statements);
            Branch(StatementList statements);
            virtual ~Branch();

            Expression* condition();

            body_iterator begin() const;
            body_iterator end() const;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            Owned<Expression> _condition;
            StatementList _statements;
    };
}

#endif // BRANCH_HPP
