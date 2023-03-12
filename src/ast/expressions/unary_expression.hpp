#ifndef KORE_UNARY_EXPRESSION_HPP
#define KORE_UNARY_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class UnaryExpression : public Expression {
        public:
            UnaryExpression(const std::string& op, Expression* expr, SourceLocation location);
            virtual ~UnaryExpression();

            std::string op() const;
            Expression* expr();

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            std::string _op;
            pointer _expr;
    };
}

#endif // KORE_UNARY_EXPRESSION_HPP
