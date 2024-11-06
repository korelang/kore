#include "ast/ast_visitor.hpp"
#include "ast/expressions/field_access_expression.hpp"

namespace kore {
    FieldAccessExpression::FieldAccessExpression(Owned<Expression> obj, Owned<Expression> field_expr)
            : Expression(ExpressionType::FieldAccess), _obj(std::move(obj)), _field_expr(std::move(field_expr)) {
        // TODO: Set location
    }

    FieldAccessExpression::~FieldAccessExpression() {}

    Expression* FieldAccessExpression::obj() {
        return _obj.get();
    }

    Expression* FieldAccessExpression::field_expr() {
        return _field_expr.get();
    }

    const Type* FieldAccessExpression::type() const {
        return _type;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(FieldAccessExpression)
}
