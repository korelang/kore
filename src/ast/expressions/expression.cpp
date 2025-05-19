#include "ast/expressions/expression.hpp"

namespace kore {
    std::ostream& operator<<(std::ostream& os, const Expression& expr) {
        return os << expr.type_name();
    }

    Expression::Expression(ExpressionType type) : _expr_type(type) {}

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
        return _type;
    }

    void Expression::set_type(const Type* type) {
        _type = type;
    }

    void Expression::set_parenthesised(bool flag) {
        _parenthesised = flag;
    }

    std::string Expression::type_name() const {
        switch (this->expr_type()) {
            case ExpressionType::Array:       return "array";
            case ExpressionType::Index:       return "index";
            case ExpressionType::Binary:      return "binary";
            case ExpressionType::Call:        return "call";
            case ExpressionType::FieldAccess: return "field_access";
            case ExpressionType::Identifier:  return "identifier";
            case ExpressionType::Error:       return "error";
            case ExpressionType::Literal:     return "literal";
            case ExpressionType::Parameter:   return "parameter";
            case ExpressionType::Unary:       return "unary";
        }
    }
}
