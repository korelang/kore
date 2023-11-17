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

    std::string operand_types_string(BinOp binop);
    bool compatible_binop_type(BinOp binop, const Type* const type);

    bool is_numeric_binop(BinOp binop);

    class BinaryExpression : public Expression {
        public:
            BinaryExpression(const std::string& op, Owned<Expression> left, Owned<Expression> right, SourceLocation location);
            virtual ~BinaryExpression();

            const Type* type() const override;
            BinOp op() const;
            std::string op_string() const;
            Expression* left() const;
            Expression* right() const;

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            BinOp _op;
            pointer _left;
            pointer _right;
    };

}

#endif // KORE_BINARY_EXPRESSION_HPP
