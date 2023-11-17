#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include <vector>

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "bin/korec/options.hpp"
#include "errors/error.hpp"
#include "types/scope.hpp"

namespace kore {
    class VariableDeclaration;
    class VariableAssignment;
    class BinaryExpression;

    class TypeChecker final : public AstVisitor {
        public:
            TypeChecker(const ParsedCommandLineArgs& args);
            virtual ~TypeChecker();

            /// Typecheck an AST
            int check(const Ast& ast);

            std::vector<errors::Error> errors();

        private:
            static constexpr int _NO_ERROR_THRESHOLD = -1;

            const ParsedCommandLineArgs* _args;
            ScopeStack _scope_stack;

            // How many errors to tolerate before bailing out
            int _error_threshold = _NO_ERROR_THRESHOLD;

            std::vector<errors::Error> _errors;

            void trace_type_checker(
                const std::string& name,
                const Type* type1 = nullptr,
                const Type* type2 = nullptr
            );

            void push_error(errors::Error error);
            void visit(BinaryExpression& binexpr) override;
            void visit(Identifier& identifier) override;

            void visit(IfStatement& statement) override;
            void visit(VariableAssignment& assignment) override;
            void visit(class Call& call) override;
            void visit(Branch& branch) override;
            void visit(Function& func) override;
            void visit(Return& ret) override;
    };
}

#endif // TYPE_CHECKER_HPP
