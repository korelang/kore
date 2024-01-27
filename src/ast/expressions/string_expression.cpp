#include "ast/ast_writer.hpp"
#include "ast/expressions/string_expression.hpp"

namespace kore {
    StringExpression::StringExpression(const std::string& value, SourceLocation location)
        : Expression(ExpressionType::Literal, location),
        _value(value) {
    }

    StringExpression::~StringExpression() {}

    std::string StringExpression::value() const noexcept {
        return _value;
    }

    const Type* StringExpression::type() const {
        return &_type;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(StringExpression)
}
