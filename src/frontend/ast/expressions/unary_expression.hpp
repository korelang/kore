#ifndef KORE_UNARY_EXPRESSION_HPP
#define KORE_UNARY_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class UnaryExpression : public Expression {
        public:
            UnaryExpression(const std::string& op, Expression* expr, Location location);
            virtual ~UnaryExpression();

            std::string op() const;
            reference expr();

            void write(AstWriter* const writer) override;

        private:
            std::string _op;
            pointer _expr;
    };
}

#endif // KORE_UNARY_EXPRESSION_HPP
