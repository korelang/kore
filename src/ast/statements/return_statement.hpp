#ifndef KORE_RETURN_STATEMENT_HPP
#define KORE_RETURN_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    class Return : public Statement {
        public:
            using iterator = ExpressionList::iterator;

        public:
            Return();
            Return(ExpressionList exprs);
            virtual ~Return();

            std::string name() const override;
            int expr_count() const noexcept;
            Expression* get_expr(int idx);
            const Expression* get_expr(int idx) const;

            iterator begin();
            iterator end();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            ExpressionList _exprs;
    };
}

#endif // KORE_RETURN_STATEMENT_HPP
