#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "bin/korec/options.hpp"
#include "diagnostics/diagnostic.hpp"
#include "errors/error.hpp"
#include "types/scope.hpp"

#include <vector>

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

            std::vector<Diagnostic> diagnostics();

        private:
            static constexpr int _NO_ERROR_THRESHOLD = -1;

            const ParsedCommandLineArgs* _args;
            ScopeStack _scope_stack;

            // How many errors to tolerate before bailing out
            int _error_threshold = _NO_ERROR_THRESHOLD;

            std::vector<Diagnostic> _diagnostics;

            void trace_type_checker(
                const std::string& name,
                const Type* type1 = nullptr,
                const Type* type2 = nullptr
            );

            void push_error(DiagnosticData&& data);

            void type_check_function_call(class Call& call, const FunctionType* func_type);

            void visit(ArrayExpression& array) override;
            void visit(IndexExpression& array_index) override;
            void visit(IndexExpression& array_index, ValueContext context) override;
            void visit(BinaryExpression& binexpr) override;
            void visit(Identifier& identifier) override;
            void visit(Identifier& expr, ValueContext context) override;

            void visit(IfStatement& statement) override;
            void visit(VariableAssignment& assignment) override;
            void visit(class Call& call) override;
            void visit(Branch& branch) override;
            void visit(Function& func) override;
            void visit(Return& ret) override;
    };
}

#endif // TYPE_CHECKER_HPP
