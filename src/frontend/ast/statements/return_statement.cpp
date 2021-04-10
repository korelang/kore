#include "ast/ast_writer.hpp"
#include "ast/statements/return_statement.hpp"

Return::Return(Expression* expr) : _expr(std::move(expr)) {}

Return::~Return() {}

void Return::write(AstWriter* const writer) {
    writer->write("return<");
    _expr->write(writer);
    writer->write(">");
    writer->newline();
}
