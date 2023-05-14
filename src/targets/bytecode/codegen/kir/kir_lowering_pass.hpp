#ifndef KORE_KIR_LOWERING_PASS_HPP
#define KORE_KIR_LOWERING_PASS_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "errors/errors.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/module.hpp"
#include "types/scope.hpp"

namespace kore {
    namespace kir {
        /// Compiler pass that lowers an AST to KIR
        ///
        /// It is heavily inspired by inko's MIR (mid-level intermediate representation)
        /// which in turn is inspired by rust's MIR
        class KirLoweringPass final : public AstVisitor {
            public:
                KirLoweringPass();

                /// Lower a fully type-checked AST to KIR
                Module lower(const Ast& ast);

                void visit(ArrayExpression& expr) override;
                /* virtual void visit(ArrayFillExpression& expr); */
                /* virtual void visit(ArrayRangeExpression& expr); */
                void visit(BinaryExpression& expr) override;
                void visit(BoolExpression& expr) override;
                /* virtual void visit(CharExpression& expr); */
                void visit(FloatExpression& expr) override;
                void visit(Identifier& expr) override;
                /* virtual void visit(Parameter& expr); */
                void visit(IntegerExpression& expr) override;
                /* virtual void visit(StringExpression& expr); */
                /* void visit(UnaryExpression& expr) override; */

                /* virtual void visit(Branch& statement); */
                void visit(kore::Function& statement) override;
                void visit(class Call& statement) override;
                void visit(IfStatement& statement) override;
                /* virtual void visit(ImportStatement& statement); */
                /* virtual void visit(ModuleStatement& statement); */
                void visit(Return& statement) override;
                void visit(VariableAssignment& statement) override;
                /* virtual void visit(VariableDeclaration& statement); */
                /* virtual void visit(ExpressionStatement& exprstmt); */

            private:
                ScopeStack _scope_stack;
                Module* _module;
                int _func_index;
                std::vector<errors::Error> _errors;
                std::vector<Reg> _register_stack;

                // Mapping from variable names to their registers
                std::unordered_map<std::string, Reg> _var_to_reg;

            private:
                Function& current_function();
                void enter_function(kore::Function& func);
                void exit_function();

                Reg visit_expression(Expression* expr);
                void check_register_state(Identifier& expr, Reg reg);
                void push_register(Reg reg);
        };
    }
}

#endif // KORE_KIR_LOWERING_PASS_HPP
