#ifndef KORE_AST_HPP
#define KORE_AST_HPP

#include <filesystem>
#include <string>
#include <vector>

#include "ast/statements/statement.hpp"

namespace fs = std::filesystem;

namespace kore {
    class AstWriter;

    /// Represents an abstract syntax tree
    class Ast {
        public:
            using iterator = std::vector<Statement::pointer>::iterator;
            using const_iterator = std::vector<Statement::pointer>::const_iterator;

            Ast();
            Ast(Ast&& ast);
            Ast(const fs::path& path);
            virtual ~Ast();

            Ast& operator=(Ast&& other);

            /// The module name for the parsed file
            std::string module_name() const;

            /// Path to the module
            fs::path path() const;

            /// Whether this module is the main module or not
            bool is_main() const noexcept;

            /// Add a statement to this AST
            void add_statement(Owned<Statement> statement);

            iterator begin();
            iterator end();

            const_iterator begin() const;
            const_iterator end() const;

            /// Write out the AST somewhere
            void write(AstWriter& writer) const;

        private:
            std::string _module_name;
            fs::path _module_path;
            std::vector<Statement::pointer> _statements;
    };
}

#endif // KORE_AST_HPP
