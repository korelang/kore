#include "ast/ast_visitor.hpp"
#include "ast/expressions/array_expression.hpp"
#include "types/array_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    ArrayExpression::ArrayExpression() : ArrayExpression(SourceLocation::unknown) {}

    ArrayExpression::ArrayExpression(const SourceLocation& location)
        : Expression(ExpressionType::Array, location)
    {}

    ArrayExpression::~ArrayExpression() {}

    void ArrayExpression::set_start_location(const SourceLocation& location) {
        _location = location;
    }

    void ArrayExpression::set_end_location(const SourceLocation& location) {
        _location.merge(location);
    }

    void ArrayExpression::add_element(Owned<Expression> expr) {
        _elements.emplace_back(std::move(expr));
    }

    bool ArrayExpression::uses_constants_only() const {
        // TODO:
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

    const ArrayType* ArrayExpression::array_type() const {
        return _type->as<const ArrayType>();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(ArrayExpression)
}
