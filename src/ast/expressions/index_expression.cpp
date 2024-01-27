#include "ast/ast_visitor.hpp"
#include "ast/expressions/index_expression.hpp"
#include "types/array_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    IndexExpression::IndexExpression()
        : Expression(ExpressionType::Index, SourceLocation::unknown) {
        set_type(Type::unknown());
    }

    IndexExpression::IndexExpression(
        Owned<Expression> array_expr,
        Owned<Expression> index_expr,
        const SourceLocation& location
    )
        : Expression(ExpressionType::Index, location),
          _array_expr(std::move(array_expr)),
          _index_expr(std::move(index_expr)) {
    }

    IndexExpression::~IndexExpression() {}

    Expression* IndexExpression::expr() {
        return _array_expr.get();
    }

    Expression* IndexExpression::index_expr() {
        return _index_expr.get();
    }

    const Type* IndexExpression::type() const {
        return _type;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_VALUE_CONTEXT_IMPL(IndexExpression)
}
