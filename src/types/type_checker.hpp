#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include <vector>

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "types/symbol_table.hpp"

namespace kore {
    /// Contains all the information about a type error
    struct TypeError {
        TypeError(const std::string& message, const Location& location);

        std::string message;

        Location location;

        // Used for e.g. multiple declaration errors to point to the previous
        // declaration
        Location other_location;
    };

    class VariableDeclaration;
    class VariableAssignment;
    class BinaryExpression;

    class TypeChecker final : public AstVisitor {
        public:
            TypeChecker(SymbolTable& symbol_table);
            virtual ~TypeChecker();

            /// Typecheck an AST
            int check(const Ast& ast);

            std::vector<TypeError> errors();

        private:
            SymbolTable& _symbol_table;

            // How many errors to tolerate before bailing out
            int _error_threshold = -1;

            std::vector<TypeError> _errors;

            void push_error(const std::string& message, const Location& location);
            void push_error(
                const std::string& message,
                const Location& location,
                const Location& otherLocation
            );

            void visit(BinaryExpression* expr) override;
            void visit(Identifier* expr) override;

            /* void visit(IfStatement* statement) override; */
            void visit(VariableAssignment* statement) override;
    };
}

#endif // TYPE_CHECKER_HPP
