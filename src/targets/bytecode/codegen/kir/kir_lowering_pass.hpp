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

                void visit_only(ArrayExpression& expr) override;
                /* virtual void visit_only(ArrayFillExpression& expr); */
                /* virtual void visit_only(ArrayRangeExpression& expr); */
                void visit_only(BinaryExpression& expr) override;
                void visit_only(BoolExpression& expr) override;
                /* virtual void visit_only(CharExpression& expr); */
                void visit_only(FloatExpression& expr) override;
                void visit_only(Identifier& expr) override;
                /* virtual void visit_only(Parameter& expr); */
                void visit_only(IntegerExpression& expr) override;
                /* virtual void visit_only(StringExpression& expr); */
                /* void visit_only(UnaryExpression& expr) override; */

                /* virtual void visit_only(Branch& statement); */
                void visit_only(kore::Function& statement) override;
                void visit_only(class Call& statement) override;
                void visit_only(IfStatement& statement) override;
                /* virtual void visit_only(ImportStatement& statement); */
                /* virtual void visit_only(ModuleStatement& statement); */
                void visit_only(Return& statement) override;
                void visit_only(VariableAssignment& statement) override;
                /* virtual void visit_only(VariableDeclaration& statement); */
                /* virtual void visit_only(ExpressionStatement& exprstmt); */

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
