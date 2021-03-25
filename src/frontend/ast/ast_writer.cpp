#include "ast/ast_writer.hpp"

AstWriter::AstWriter() : _indent(0) {}

AstWriter::~AstWriter() {}

void AstWriter::indent() {
    ++_indent;
}

void AstWriter::dedent() {
    if (_indent > 0) {
        --_indent;
    }
}
