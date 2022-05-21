#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/binary_expression.hpp"

#include <iostream>
#include <map>

namespace kore {
    std::map<std::string, BinOp> _STRING_TO_BINOPS{
        {"+",   BinOp::Plus},
        {"-",   BinOp::Minus},
        {"*",   BinOp::Mult},
        {"**",  BinOp::Pow},
        {"/",   BinOp::Div},
        {"%",   BinOp::Mod},
        {"rem", BinOp::Remainder},
        {"<",   BinOp::Lt},
        {"<=",  BinOp::Le},
        {">",   BinOp::Gt},
        {">=",  BinOp::Ge},
        {"==",  BinOp::Equal},
        {"!=",  BinOp::NotEqual},
        {"or",  BinOp::Or},
        {"and", BinOp::And},
        {"!",   BinOp::Not},
        {"|",   BinOp::BinOr},
        {"&",   BinOp::BinAnd},
        {"^",   BinOp::BinXor},
        {"<<",  BinOp::BinLeftShift},
        {">>",  BinOp::BinRightShift},
        {"??",  BinOp::OptionalCoalesce},
    };

    BinOp string_to_binop(const std::string& op) {
        return _STRING_TO_BINOPS[op];
    }

    std::string binop_to_string(BinOp binop) {
        switch (binop) {
            case BinOp::Plus: return "+";
            case BinOp::Minus: return "-";
            case BinOp::Mult: return "*";
            case BinOp::Pow: return "**";
            case BinOp::Div: return "/";
            case BinOp::Mod:  return "%";
            case BinOp::Remainder: return "rem";
            case BinOp::Lt: return "<";
            case BinOp::Le: return "<=";
            case BinOp::Gt: return ">";
            case BinOp::Ge: return ">=";
            case BinOp::Equal: return "=";
            case BinOp::NotEqual: return "!=";
            case BinOp::Or: return "or";
            case BinOp::And: return "and";
            case BinOp::Not: return "!";
            case BinOp::BinOr: return "|";
            case BinOp::BinAnd: return "&";
            case BinOp::BinXor: return "^";
            case BinOp::BinLeftShift: return "<<";
            case BinOp::BinRightShift: return ">>";
            case BinOp::OptionalCoalesce: return "??";
        }
    }

    std::ostream& operator<<(std::ostream& os, BinOp binop) {
        return os << binop_to_string(binop);
    }

    BinaryExpression::BinaryExpression(
        const std::string& op,
        Expression* left,
        Expression* right,
        Location location
    ) : Expression(ExpressionType::Binary, location),
        _op(string_to_binop(op)),
        _left(left),
        _right(right)
    {}

    BinaryExpression::~BinaryExpression() {
    }

    const Type* BinaryExpression::type() const {
        return _type;
    }

    BinOp BinaryExpression::op() const {
        return _op;
    }

    std::string BinaryExpression::op_string() const {
        return binop_to_string(_op);
    }

    Expression* BinaryExpression::left() const {
        return _left.get();
    }

    Expression* BinaryExpression::right() const {
        return _right.get();
    }

    void BinaryExpression::accept(AstVisitor& visitor) {
        _left->accept(visitor);
        _right->accept(visitor);

        visitor.visit(*this);
    }

    void BinaryExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
