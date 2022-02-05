#ifndef KORE_BINARY_EXPRESSION_HPP
#define KORE_BINARY_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class AstWriter;
    class AstVisitor;

    class BinaryExpression : public Expression {
        public:
            BinaryExpression(const std::string& op, Expression* left, Expression* right, Location location);
            virtual ~BinaryExpression();

            const Type* type() const override;
            std::string op() const;
            Expression* left() const;
            Expression* right() const;

            void write(AstWriter* const writer) override;
            void accept(AstVisitor* visitor) override;

        private:
            std::string _op;
            pointer _left;
            pointer _right;
    };

}

#endif // KORE_BINARY_EXPRESSION_HPP
