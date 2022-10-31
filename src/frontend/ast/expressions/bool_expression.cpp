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

    const Type* BoolExpression::type() const {
        return &_type;
    }

    void BoolExpression::accept(AstVisitor& visitor) {
        accept_visit_only(visitor);
    }

    void BoolExpression::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
