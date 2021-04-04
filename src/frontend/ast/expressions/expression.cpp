#include "ast/expressions/array_fill_expression.hpp"
#include "ast/expressions/array_expression.hpp"
#include "ast/expressions/array_range_expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/bool_expression.hpp"
#include "ast/expressions/char_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/float_expression.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/expressions/integer_expression.hpp"
#include "ast/expressions/string_expression.hpp"
#include "ast/expressions/unary_expression.hpp"
#include "ast/parser_error_node.hpp"

Expression::Expression(ExpressionType type, const Location& location)
    : AstNode(location),
      _type(type) {
}

Expression::~Expression() {}

bool Expression::is_error() const noexcept {
    return false;
}

Type* Expression::type() const {
    return nullptr;
}

/* Statement* Expression::as_statement() const { */
/*     return ExpressionStatement(this); */
/* } */

void Expression::set_parenthesised(bool flag) {
    _parenthesised = flag;
}

Expression* Expression::make_parser_error(const std::string& msg, const Location& location) {
    return new ParserErrorNode(msg, location);
}

Expression* Expression::make_array_fill(
    Expression* size_expr,
    Expression* fill_expr,
    const Location& location
) {
    return new ArrayFillExpression(size_expr, fill_expr, location);
}

Expression* Expression::make_empty_array() {
    return new ArrayExpression();
}

Expression* Expression::make_array_range(
    Expression* start_expr,
    Expression* end_expr,
    const Location& location
) {
    return new ArrayRangeExpression(start_expr, end_expr, location);
}

Expression* Expression::make_bool_literal(const std::string& value, const Location& location) {
    return new BoolExpression(value, location);
}

Expression* Expression::make_int_literal(i32 value, const Location& location) {
    // TODO: Should this just take the token string and convert itself?
    return new IntegerExpression(value, location);
}

Expression* Expression::make_float_literal(f32 value, const Location& location) {
    return new FloatExpression(value, location);
}

Expression* Expression::make_char_literal(i32 codepoint, const Location& location) {
    return new CharExpression(codepoint, location);
}

Expression* Expression::make_string_literal(const std::string& str, const Location& location) {
    return new StringExpression(str, location);
}

Identifier* Expression::make_identifier(const Token& token) {
    return new Identifier(token.value(), token.location());
    /* return Expression::make_identifier(token.value(), token.location()); */
}

Identifier* Expression::make_identifier(const std::string& str, const Location& location) {
    return new Identifier(str, location);
}

Identifier* Expression::make_identifier(const std::vector<std::string>& parts, const Location& location) {
    return new Identifier(parts, location);
}

Expression* Expression::make_unary(const std::string& op, Expression* expr, const Location& location) {
    return new UnaryExpression(op, expr, location);
}

Expression* Expression::make_binary(
    const std::string& op,
    Expression* left,
    Expression* right,
    const Location& location
) {
    return new BinaryExpression(op, left, right, location);
}
