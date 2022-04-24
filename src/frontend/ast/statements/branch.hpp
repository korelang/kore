#ifndef BRANCH_HPP
#define BRANCH_HPP

#include "ast/statements/statement_list.hpp"

namespace kore {
    using body_iterator = std::vector<std::unique_ptr<Statement>>::const_iterator;

    class Branch : public Statement {
        public:
            Branch(Expression* condition, StatementList statements);
            Branch(StatementList statements);
            virtual ~Branch();

            Expression* condition();

            body_iterator begin() const;
            body_iterator end() const;

            void write(AstWriter* const writer) override;
            void accept(AstVisitor& visitor) override;

        private:
            Expression* _condition;
            StatementList _statements;
    };
}

#endif // BRANCH_HPP
