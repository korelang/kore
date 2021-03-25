#include "ast/ast_writer.hpp"
#include "ast/statements/import_statement.hpp"

ImportStatement::ImportStatement(const Token& token)
    : ImportStatement(token.value(), token.location()) {
}

ImportStatement::ImportStatement(const std::string& import_spec, const Location& location)
    : Statement(location),
      _spec(import_spec) {
}

ImportStatement::~ImportStatement() {}

std::string ImportStatement::spec() const {
    return _spec;
}

void ImportStatement::write(AstWriter* const writer) {
    writer->write("import ");
    writer->write(_spec);
    writer->newline();
}
