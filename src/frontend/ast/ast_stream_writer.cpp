#include "ast/ast_stream_writer.hpp"

namespace kore {
    AstStreamWriter::AstStreamWriter() : _out(nullptr) {}

    AstStreamWriter::AstStreamWriter(std::ostream& out) {
        _out = &out;
    }

    AstStreamWriter::~AstStreamWriter() {
        _out = nullptr;
    }

    void AstStreamWriter::write(const std::string& value) {
        if (_out) {
            *_out << value;
        }
    }

    void AstStreamWriter::write(const Ast& ast) {
        if (_out) {
            ast.write(this);
            *_out << std::endl;
        }
    }

    void AstStreamWriter::newline() {
        if (_out) {
            *_out << std::endl;
        }
    }
}
