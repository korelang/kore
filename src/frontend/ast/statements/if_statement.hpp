#ifndef IF_STATEMENT_HPP
#define IF_STATEMENT_HPP

#include "ast/statements/branch.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    class IfStatement : public Statement {
        public:
            IfStatement();
            virtual ~IfStatement();

            void add_branch(Expression* condition);
            void add_else_branch();

            void add_statement(Statement* statement) override;
            bool has_else_branch() const;

            void write(AstWriter* const writer) override;

        private:
            bool _has_else_branch;
            std::vector<std::unique_ptr<Branch>> branches;
            StatementList statement_accumulator;
    };
}

#endif // IF_STATEMENTIF_STATEMENT_HPP
