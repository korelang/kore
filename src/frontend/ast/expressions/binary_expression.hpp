#ifndef KORE_BINARY_EXPRESSION_HPP
#define KORE_BINARY_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
    class AstWriter;
    class AstVisitor;

    enum class BinOp {
        Plus,
        Minus,
        Mult,
        Pow,
        Div,
        Mod,
        Remainder,
        Lt,
        Le,
        Gt,
        Ge,
        Equal,
        NotEqual,
        Or,
        And,
        Not,
        BinOr,
        BinAnd,
        BinXor,
        BinLeftShift,
        BinRightShift,
        OptionalCoalesce,
    };

    BinOp binop_from_string(const std::string& op);
    std::string binop_to_string(BinOp binop);

    class BinaryExpression : public Expression {
        public:
            BinaryExpression(const std::string& op, Expression* left, Expression* right, Location location);
            virtual ~BinaryExpression();

            const Type* type() const override;
            BinOp op() const;
            Expression* left() const;
            Expression* right() const;

            void write(AstWriter* const writer) override;
            void accept(AstVisitor& visitor) override;

        private:
            BinOp _op;
            pointer _left;
            pointer _right;
    };

}

#endif // KORE_BINARY_EXPRESSION_HPP
