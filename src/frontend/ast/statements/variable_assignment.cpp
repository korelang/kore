#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/expressions/expression.hpp"
#include "types/type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    VariableAssignment::VariableAssignment(
        const Token& identifier,
        Type* type,
        Expression* expr
    )
        : Statement(identifier.location(), StatementType::VariableAssignment),
        _identifier(identifier),
        _type(std::move(type)),
        _expr(std::move(expr))
    {
        _location = Location(
            identifier.location().lnum(),
            identifier.location().start(),
            expr->location().end()
        );

        _identifier.set_type(type);
    }

    VariableAssignment::~VariableAssignment() {}

    const Identifier* VariableAssignment::identifier() const {
        return &_identifier;
    }

    Expression* VariableAssignment::expression() const {
        return _expr.get();
    }

    void VariableAssignment::set_type(const Type* type) {
        _identifier.set_type(type);
    }

    const Type* VariableAssignment::type() const {
        return _identifier.type();
    }

    const Type* VariableAssignment::declared_type() const {
        return _type.get();
    }

    void VariableAssignment::write(AstWriter* const writer) {
        writer->write("variable_assignment<");
        writer->write(identifier()->name());
        writer->write(" ");
        /* _type->write(writer); */
        writer->write(" = ");
        _expr->write(writer);
        writer->write(">");
        writer->newline();
    }

    void VariableAssignment::accept(AstVisitor* visitor) {
        _expr->accept(visitor);
        /* _identifier.accept(visitor); */

        visitor->visit(this);
    }
}
