#include "ast/ast_writer.hpp"
#include "ast/expressions/binary_expression.hpp"

BinaryExpression::BinaryExpression(
    const std::string& op,
    Expression* left,
    Expression* right,
    Location location
) : Expression(ExpressionType::Binary, location),
    _op(op),
    _left(left),
    _right(right)
{}

BinaryExpression::~BinaryExpression() {
}

std::string BinaryExpression::op() const {
    return _op;
}

const Expression* BinaryExpression::left() const {
    return _left.get();
}

const Expression* BinaryExpression::right() const {
    return _right.get();
}

void BinaryExpression::write(AstWriter* const writer) {
    writer->write("(");//writer << "(";
    _left->write(writer);
    writer->write(op());//writer << op();
    _right->write(writer);
    writer->write(")");//writer << ")";
}
