#ifndef KORE_BINARY_EXPRESSION_HPP
#define KORE_BINARY_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

class AstWriter;

class BinaryExpression : public Expression {
    public:
        BinaryExpression(const std::string& op, Expression* left, Expression* right, Location location);
        virtual ~BinaryExpression();

        std::string op() const;
        const Expression* left() const;
        const Expression* right() const;

        void write(AstWriter* const writer) override;

    private:
        std::string _op;
        pointer _left;
        pointer _right;
};


#endif // KORE_BINARY_EXPRESSION_HPP
