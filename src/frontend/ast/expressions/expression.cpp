#include "ast/expressions/expression.hpp"

namespace kore {
    std::ostream& operator<<(std::ostream& os, ExpressionType expr_type) {
        switch (expr_type) {
            case ExpressionType::Array:      os << "array"; break;
            case ExpressionType::Binary:     os << "binary"; break;
            case ExpressionType::Call:       os << "call"; break;
            case ExpressionType::Identifier: os << "identifier"; break;
            case ExpressionType::Error:      os << "error"; break;
            case ExpressionType::Literal:    os << "literal"; break;
            case ExpressionType::Parameter:  os << "parameter"; break;
            case ExpressionType::Unary:      os << "unary"; break;
        }

        return os;
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
