#ifndef KORE_AST_WRITER_HPP
#define KORE_AST_WRITER_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"

namespace kore {
    /// Base class for all AST writers
    class AstWriter : public AstVisitor {
        public:
            AstWriter();
            virtual ~AstWriter();

            /// Write a string to an output stream
            virtual void write(const std::string& value) = 0;

            /// Write an AST to an output stream
            virtual void write(const Ast& ast) = 0;

        protected:
            int _indent;

            void indent();
            void dedent();
            virtual void newline(int count = 1) = 0;
    };
}

#endif // KORE_AST_WRITER_HPP
