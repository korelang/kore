#ifndef TYPE_INFERRER_HPP
#define TYPE_INFERRER_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "bin/korec/options.hpp"
#include "types/scope.hpp"

namespace kore {
    /// Class that tries to infer the types of nodes in an AST
    class TypeInferrer final : public AstVisitor {
        public:
            TypeInferrer(const ParsedCommandLineArgs& args);
            virtual ~TypeInferrer();

            void infer(Ast& ast);

        private:
            const ParsedCommandLineArgs* _args;
            ScopeStack _scope_stack;

            void trace_type_inference(const std::string& name, const Type* type = nullptr);

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
