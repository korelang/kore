#include "ast/ast_writer.hpp"
#include "ast/expressions/string_expression.hpp"

namespace kore {
    StringExpression::StringExpression(const std::string& value, Location location)
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

    void StringExpression::accept(AstVisitor& visitor) {
        accept_visit_only(visitor);
    }

    void StringExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
