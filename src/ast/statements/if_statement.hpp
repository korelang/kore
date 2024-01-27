#ifndef IF_STATEMENT_HPP
#define IF_STATEMENT_HPP

#include "ast/statements/branch.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/expressions/expression.hpp"
#include "pointer_types.hpp"

namespace kore {
    using branch_iterator = std::vector<Owned<Branch>>::const_iterator;

    class IfStatement : public Statement {
        public:
            IfStatement();
            virtual ~IfStatement();

            void add_branch(Owned<Expression> condition);
            void add_else_branch();

            void add_statement(Owned<Statement> statement) override;
            bool has_else_branch() const;

            int branch_count() const;
            branch_iterator begin() const;
            branch_iterator end() const;
            Branch* operator[](int index);
            Branch* last_branch();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            bool _has_else_branch;
            std::vector<Owned<Branch>> _branches;
            StatementList _statement_accumulator;
    };
}

#endif // IF_STATEMENT_HPP
