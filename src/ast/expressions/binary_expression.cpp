#include "ast/ast_visitor.hpp"
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
            case BinOp::Plus:             return "+";
            case BinOp::Minus:            return "-";
            case BinOp::Mult:             return "*";
            case BinOp::Pow:              return "**";
            case BinOp::Div:              return "/";
            case BinOp::Mod:              return "%";
            case BinOp::Remainder:        return "rem";
            case BinOp::Lt:               return "<";
            case BinOp::Le:               return "<=";
            case BinOp::Gt:               return ">";
            case BinOp::Ge:               return ">=";
            case BinOp::Equal:            return "=";
            case BinOp::NotEqual:         return "!=";
            case BinOp::Or:               return "or";
            case BinOp::And:              return "and";
            case BinOp::Not:              return "!";
            case BinOp::BinOr:            return "|";
            case BinOp::BinAnd:           return "&";
            case BinOp::BinXor:           return "^";
            case BinOp::BinLeftShift:     return "<<";
            case BinOp::BinRightShift:    return ">>";
            case BinOp::OptionalCoalesce: return "??";
        }
    }

    std::string operand_types_string(BinOp binop) {
        switch (binop) {
            case BinOp::Plus:
            case BinOp::Minus:
            case BinOp::Mult:
            case BinOp::Div:
            case BinOp::Mod:
            case BinOp::Remainder:
            case BinOp::Lt:
            case BinOp::Le:
            case BinOp::Gt:
            case BinOp::Ge:
            case BinOp::BinOr:
            case BinOp::BinAnd:
            case BinOp::BinXor:
            case BinOp::BinLeftShift:
            case BinOp::BinRightShift: {
                return "numeric";
            }

            case BinOp::Equal:
            case BinOp::NotEqual: {
                // We can always compare any type
                return "";
            }

            case BinOp::Or:
            case BinOp::And:
            case BinOp::Not: {
                return "bool";
            }

            default:
                return "";
        }

        return "";
    }

    bool compatible_binop_type(BinOp binop, const Type* const type) {
        switch (binop) {
            case BinOp::Plus:
            case BinOp::Minus:
            case BinOp::Mult:
            case BinOp::Div:
            case BinOp::Mod:
            case BinOp::Remainder:
            case BinOp::Lt:
            case BinOp::Le:
            case BinOp::Gt:
            case BinOp::Ge:
            case BinOp::BinOr:
            case BinOp::BinAnd:
            case BinOp::BinXor:
            case BinOp::BinLeftShift:
            case BinOp::BinRightShift: {
                return type->is_numeric();
            }

            case BinOp::Equal:
            case BinOp::NotEqual: {
                // We can always compare any type
                return true;
            }

            case BinOp::Or:
            case BinOp::And:
            case BinOp::Not: {
                return type->category() == TypeCategory::Bool;
            }

            default:
                return false;
        }

        return false;
    }

    bool is_numeric_binop(BinOp binop) {
        switch (binop) {
            case BinOp::Plus:
            case BinOp::Minus:
            case BinOp::Mult:
            case BinOp::Div:
            case BinOp::Mod:
            case BinOp::Remainder:
            case BinOp::Lt:
            case BinOp::Le:
            case BinOp::Gt:
            case BinOp::Ge:
                return true;

            default:
                return false;
        }
    }

    std::ostream& operator<<(std::ostream& os, BinOp binop) {
        return os << binop_to_string(binop);
    }

    BinaryExpression::BinaryExpression(
        const std::string& op,
        Owned<Expression> left,
        Owned<Expression> right,
        SourceLocation location
    ) : Expression(ExpressionType::Binary, location),
        _op(string_to_binop(op)),
        _left(std::move(left)),
        _right(std::move(right))
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
