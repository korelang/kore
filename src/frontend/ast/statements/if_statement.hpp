#ifndef IF_STATEMENT_HPP
#define IF_STATEMENT_HPP

#include "ast/statements/branch.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    using branch_iterator = std::iterator<std::input_iterator_tag, Branch*>;

    class IfStatement : public Statement {
        public:
            IfStatement();
            virtual ~IfStatement();

            void add_branch(Expression* condition);
            void add_else_branch();

            void add_statement(Statement* statement) override;
            bool has_else_branch() const;

            branch_iterator branches();

            void write(AstWriter* const writer) override;
            void accept(AstVisitor* visitor) override;

        private:
            bool _has_else_branch;
            std::vector<std::unique_ptr<Branch>> _branches;
            StatementList _statement_accumulator;
    };
}

#endif // IF_STATEMENT_HPP
