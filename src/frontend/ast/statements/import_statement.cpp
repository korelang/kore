#include "ast/ast_writer.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/expressions/identifier.hpp"

ImportStatement::ImportStatement(Identifier* import_spec)
    : Statement(Location::unknown, StatementType::ImportDecl),
      _spec(std::move(import_spec)) {
}

ImportStatement::~ImportStatement() {}

std::string ImportStatement::spec() const {
    return _spec->qualified_name();
}

void ImportStatement::write(AstWriter* const writer) {
    writer->write("import ");
    _spec->write(writer);
    writer->newline();
}
