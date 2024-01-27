#include "ast/ast_visitor.hpp"
#include "ast/expressions/array_expression.hpp"
#include "types/array_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    ArrayExpression::ArrayExpression()
        : Expression(ExpressionType::Array, SourceLocation::unknown),
        _start(SourceLocation::unknown),
        _end(SourceLocation::unknown),
        _type(Type::make_array_type(Type::unknown())) {
    }

    ArrayExpression::ArrayExpression(const SourceLocation& location)
        : Expression(ExpressionType::Array, location),
        _start(location),
        _end(SourceLocation::unknown) {
    }

    ArrayExpression::~ArrayExpression() {}

    void ArrayExpression::set_start_location(const SourceLocation& location) {
        _start = location;
    }

    void ArrayExpression::set_end_location(const SourceLocation& location) {
        _end = location;
    }

    void ArrayExpression::add_element(Owned<Expression> expr) {
        if (_elements.empty()) {
            _type = Type::make_array_type(expr->type());
        } else {
            _type->unify_element_type(expr->type());
        }

        _elements.emplace_back(std::move(expr));
    }

    bool ArrayExpression::uses_constants_only() const {
        return false;
    }

    int ArrayExpression::size() const {
        return _elements.size();
    }

    Expression* ArrayExpression::operator[](int index) {
        /* KORE_DEBUG_ASSERT(index >= 0 && index < size(), "Array expression index out of range"); */

        return _elements[index].get();
    }

    const Type* ArrayExpression::type() const {
        return _type;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(ArrayExpression)
}
