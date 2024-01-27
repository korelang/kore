#include "ast/expressions/expression.hpp"

namespace kore {
    std::ostream& operator<<(std::ostream& os, ExpressionType expr_type) {
        switch (expr_type) {
            case ExpressionType::Array:      return os << "array";
            case ExpressionType::Index:      return os << "index";
            case ExpressionType::Binary:     return os << "binary";
            case ExpressionType::Call:       return os << "call";
            case ExpressionType::Identifier: return os << "identifier";
            case ExpressionType::Error:      return os << "error";
            case ExpressionType::Literal:    return os << "literal";
            case ExpressionType::Parameter:  return os << "parameter";
            case ExpressionType::Unary:      return os << "unary";
        }
    }

    Expression::Expression(ExpressionType type, const SourceLocation& location)
        : AstNode(location),
        _expr_type(type) {
    }

    Expression::~Expression() {}

    bool Expression::is_error() const noexcept {
        return expr_type() == ExpressionType::Error;
    }

    bool Expression::is_literal() const noexcept {
        return expr_type() == ExpressionType::Literal;
    }

    bool Expression::is_identifier() const noexcept {
        return expr_type() == ExpressionType::Identifier;
    }

    ExpressionType Expression::expr_type() const {
        return _expr_type;
    }

    const Type* Expression::type() const {
        throw std::runtime_error("Called unimplemented Expression::type method");
    }

    void Expression::set_type(const Type* type) {
        _type = type;
    }

    void Expression::set_parenthesised(bool flag) {
        _parenthesised = flag;
    }
}
