#include "ast/ast_writer.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/expressions/expression.hpp"
#include "types/type.hpp"

VariableAssignment::VariableAssignment(
    const Token& identifier,
    Type* type,
    Expression* expr
)
    : Statement(),
      _identifier(identifier.value()),
      _type(std::move(type)),
      _expr(std::move(expr))
{
    _location = Location(
        identifier.location().lnum(),
        identifier.location().start(),
        expr->location().end()
    );
}

VariableAssignment::~VariableAssignment() {}

std::string VariableAssignment::identifier() const {
    return _identifier;
}

const Type* VariableAssignment::type() const {
    return _type.get();
}

void VariableAssignment::write(AstWriter* const writer) {
    writer->write("variable_assignment<");
    writer->write(identifier());
    writer->write(" ");
    _type->write(writer);
    writer->write(" = ");
    _expr->write(writer);
    writer->write(">");
    writer->newline();
}
