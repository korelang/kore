#ifndef TYPE_INFERRER_HPP
#define TYPE_INFERRER_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "types/scope.hpp"

namespace kore {
    /// Class that tries to infer the types of nodes in an AST
    class TypeInferrer final : public AstVisitor {
        public:
            TypeInferrer();
            virtual ~TypeInferrer();

            void infer(Ast& ast);

        private:
            // We need a symbol table for inference of the types of variables
            ScopeStack _scope_stack;

            void visit(BinaryExpression& expr) override;
            void visit(class Call& call) override;
            void visit(Identifier& expr) override;
            void visit(UnaryExpression& expr) override;

            void visit(VariableAssignment& statement) override;
            void visit(Branch& branch) override;
            void visit(Function& func) override;
            void visit(Return& ret) override;
    };
}

#endif // TYPE_INFERRER_HPP
