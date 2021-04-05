#include "ast/ast_writer.hpp"
#include "ast/statements/variable_declaration.hpp"

VariableDeclaration::VariableDeclaration(const Token& identifier, const Token& type)
    : Statement(),
      _identifier(identifier.value()),
      _type(type.value())
{
    _location = Location(
        identifier.location().lnum(),
        identifier.location().start(),
        type.location().end()
    );
}

VariableDeclaration::~VariableDeclaration() {}

std::string VariableDeclaration::identifier() const {
    return _identifier;
}

std::string VariableDeclaration::type() const {
    return _type;
}

void VariableDeclaration::write(AstWriter* const writer) {
    writer->write("variable_declaration<");
    writer->write(identifier());
    writer->write(" ");
    writer->write(type());
    writer->write(">");
    writer->newline();
}
