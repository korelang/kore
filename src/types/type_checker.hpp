#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include <vector>

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "errors/error.hpp"
#include "types/scope.hpp"

namespace kore {
    class VariableDeclaration;
    class VariableAssignment;
    class BinaryExpression;

    class TypeChecker final : public AstVisitor {
        public:
            TypeChecker(ScopeStack& scope_stack);
            virtual ~TypeChecker();

            /// Typecheck an AST
            int check(const Ast& ast);

            std::vector<errors::Error> errors();

        private:
            static constexpr int _NO_ERROR_THRESHOLD = -1;

            ScopeStack& _scope_stack;

            // How many errors to tolerate before bailing out
            int _error_threshold = _NO_ERROR_THRESHOLD;

            std::vector<errors::Error> _errors;

            void push_error(errors::Error error);
            void visit(BinaryExpression& binexpr) override;
            void visit(Identifier& identifier) override;

            /* void visit(IfStatement& statement) override; */
            void visit(VariableAssignment& assignment) override;
            void visit(class Call& call) override;
            void visit(Branch& branch) override;
            void visit(Function& func) override;
            void visit(Return& ret) override;

            /// Check if a variable shadows a variable in an outer scope
            bool shadows_outer_scope(const Identifier& identifier);
    };
}

#endif // TYPE_CHECKER_HPP
