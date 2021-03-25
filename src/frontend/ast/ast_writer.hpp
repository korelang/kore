#ifndef KORE_AST_WRITER_HPP
#define KORE_AST_WRITER_HPP

#include "ast/ast.hpp"

/// Base class for all AST writers
class AstWriter {
    public:
        AstWriter();
        virtual ~AstWriter();

        void indent();
        void dedent();

        /// Write a string to an output stream
        virtual void write(const std::string& value) = 0;

        /// Write an AST to an output stream
        virtual void write(const Ast& ast) = 0;

        virtual void newline() = 0;

    private:
        int _indent;
};

#endif // KORE_AST_WRITER_HPP
