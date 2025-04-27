#ifndef TYPE_INFERRER_HPP
#define TYPE_INFERRER_HPP

#include "ast/ast.hpp"
#include "ast/ast_expr_visitor.hpp"
#include "ast/ast_statement_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "bin/korec/options.hpp"
#include "types/scope.hpp"

namespace kore {
    /// Class that tries to infer the types of nodes in an AST
    class TypeInferrer final : public AstExprVisitor, AstStatementVisitor {
        // using AstExprVisitor::visit;
        // using AstStatementVisitor::visit;

        public:
            TypeInferrer(const ParsedCommandLineArgs& args);
            virtual ~TypeInferrer();

            void infer(Ast& ast);

        private:
            const ParsedCommandLineArgs* _args;
            ScopeStack _scope_stack;

            void trace_type_inference(const std::string& name, const Type* type = nullptr);

            void visit(ArrayExpression& array) override;
            void visit(IndexExpression& array_index) override;
            void visit(IndexExpression& array_index, ValueContext context) override;
            void visit(BinaryExpression& expr) override;
            void visit(class Call& call) override;
            void visit(Identifier& expr) override;
            void visit(Identifier& expr, ValueContext context) override;
            void visit(UnaryExpression& expr) override;

            void visit(VariableAssignment& statement) override;
            void visit(Branch& branch) override;
            void visit(Function& func) override;
            void visit(Return& ret) override;
    };
}

#endif // TYPE_INFERRER_HPP
