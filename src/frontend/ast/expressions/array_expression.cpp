#include "ast/ast_writer.hpp"
#include "ast/expressions/array_expression.hpp"

ArrayExpression::ArrayExpression()
    : Expression(ExpressionType::array, Location::unknown),
      _start(Location::unknown),
      _end(Location::unknown) {
}

ArrayExpression::ArrayExpression(const Location& location)
    : Expression(ExpressionType::array, location),
      _start(location),
      _end(Location::unknown) {
}

ArrayExpression::~ArrayExpression() {}

void ArrayExpression::set_start_location(const Location& location) {
    _start = location;
}

void ArrayExpression::set_end_location(const Location& location) {
    _end = location;
}

void ArrayExpression::add_element(Expression* expr) {
    _elements.emplace_back(std::move(expr));
}

bool ArrayExpression::uses_constants_only() const {
    return false;
}

void ArrayExpression::write(AstWriter* const writer) {
    writer->write("array<");

    auto it = std::cbegin(_elements);
    auto last = std::prev(std::cend(_elements));

    for (; it < last; it = std::next(it)) {
        auto& element = *it;

        element->write(writer);
        writer->write(", ");
    }

    (*it)->write(writer);
    writer->write(">");
}
