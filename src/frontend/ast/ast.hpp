#ifndef KORE_AST_HPP
#define KORE_AST_HPP

#include <memory>
#include <string>
#include <vector>

#include "ast/statements/statement.hpp"

namespace kore {
    class AstWriter;

    /// Represents an abstract syntax tree
    class Ast {
        public:
            using ConstIter = std::vector<std::unique_ptr<Statement>>::const_iterator;

            Ast();
            Ast(Ast&& ast);
            virtual ~Ast();

            /// The module name for the parsed file
            std::string module_name() const;

            /// Whether this module is the main module or not
            bool is_main() const noexcept;

            /// Add a statement to this AST
            void add_statement(Statement* statement);

            ConstIter begin() const;
            ConstIter end() const;

            /// Write out the AST somewhere
            void write(AstWriter* const writer) const;

        private:
            std::string _module_name;
            std::vector<std::unique_ptr<Statement>> _statements;
    };
}

#endif // KORE_AST_HPP
