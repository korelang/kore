#include "ast/ast_writer.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/expressions/expression.hpp"

VariableAssignment::VariableAssignment(const Token& identifier, const Token& type, Expression* expr)
    : Statement(),
      _identifier(identifier.value()),
      _type(type.value()),
      _expr(std::move(expr))
{
    _location = Location(
        identifier.location().lnum(),
        identifier.location().start(),
        type.location().end()
    );
}

VariableAssignment::~VariableAssignment() {}

std::string VariableAssignment::identifier() const {
    return _identifier;
}

std::string VariableAssignment::type() const {
    return _type;
}

void VariableAssignment::write(AstWriter* const writer) {
    writer->write("variable_assignment<");
    writer->write(identifier());
    writer->write(" ");
    writer->write(type());
    writer->write(" = ");
    _expr->write(writer);
    writer->write(">");
    writer->newline();
}
