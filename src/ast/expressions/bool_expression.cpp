#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/bool_expression.hpp"

namespace kore {
    BoolExpression::BoolExpression(const std::string& value, const SourceLocation& location)
        : Expression(ExpressionType::Literal, location),
        _value(value) {
    }

    BoolExpression::~BoolExpression() {}

    std::string BoolExpression::value() const {
        return _value;
    }

    bool BoolExpression::bool_value() const {
        return value() == "true";
    }

    const Type* BoolExpression::type() const {
        return &_type;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(BoolExpression)
}
