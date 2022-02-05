#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/binary_expression.hpp"

namespace kore {
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

    const Type* BinaryExpression::type() const {
        return _type;
    }

    std::string BinaryExpression::op() const {
        return _op;
    }

    Expression* BinaryExpression::left() const {
        return _left.get();
    }

    Expression* BinaryExpression::right() const {
        return _right.get();
    }

    void BinaryExpression::write(AstWriter* const writer) {
        writer->write("(");//writer << "(";
        _left->write(writer);
        writer->write(op());//writer << op();
        _right->write(writer);
        writer->write(")");//writer << ")";
    }

    void BinaryExpression::accept(AstVisitor* visitor) {
        if (visitor->precondition(this)) {
            return;
        }

        _left->accept(visitor);
        _right->accept(visitor);

        visitor->visit(this);
    }
}
