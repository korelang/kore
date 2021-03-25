#ifndef KORE_AST_STREAM_WRITER_HPP
#define KORE_AST_STREAM_WRITER_HPP

#include <ostream>

#include "ast/ast_writer.hpp"

/// Writes an abstract syntax tree to an output stream
class AstStreamWriter : public AstWriter {
    public:
        AstStreamWriter();
        AstStreamWriter(std::ostream& out);
        virtual ~AstStreamWriter();

        void write(const std::string& value) override;
        void write(const Ast& ast) override;
        void newline() override;

    private:
        std::ostream* _out;
};

#endif // KORE_AST_STREAM_WRITER_HPP
