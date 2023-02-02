#ifndef IF_STATEMENT_HPP
#define IF_STATEMENT_HPP

#include "ast/statements/branch.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/expressions/expression.hpp"
#include "pointer_types.hpp"

namespace kore {
    using branch_ptr = Owned<Branch>;
    using branch_iterator = std::vector<branch_ptr>::const_iterator;

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
            branch_ptr& operator[](int index);
            branch_ptr& last_branch();

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            bool _has_else_branch;
            std::vector<branch_ptr> _branches;
            StatementList _statement_accumulator;
    };
}

#endif // IF_STATEMENT_HPP
