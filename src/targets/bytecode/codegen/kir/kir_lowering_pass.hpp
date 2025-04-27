#ifndef KORE_KIR_LOWERING_PASS_HPP
#define KORE_KIR_LOWERING_PASS_HPP

#include "analysis/function_name_visitor.hpp"
#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/expressions/index_expression.hpp"
#include "bin/korec/options.hpp"
#include "errors/errors.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/module.hpp"
#include "types/scope.hpp"

namespace kore {
    namespace kir {
        using Regs = std::vector<Reg>;

        /// Compiler pass that lowers an AST to KIR
        ///
        /// It is heavily inspired by inko's MIR (mid-level intermediate representation)
        /// which in turn is inspired by rust's MIR
        class KirLoweringPass final : public AstVisitor {
            public:
                KirLoweringPass(const ParsedCommandLineArgs& args);

                /// Lower a fully type-checked AST to KIR
                Module lower(const Ast& ast);

                void visit(ArrayExpression& expr) override;
                // void visit(ArrayFillExpression& expr) override;
                // void visit(ArrayRangeExpression& expr) override;
                void visit(IndexExpression& expr) override;
                void visit(IndexExpression& expr, ValueContext context) override;
                void visit(BinaryExpression& expr) override;
                void visit(BoolExpression& expr) override;
                void visit(FloatExpression& expr) override;
                void visit(Identifier& expr) override;
                void visit(Identifier& expr, ValueContext context) override;
                void visit(IntegerExpression& expr) override;

                void visit(kore::Function& statement) override;
                void visit(class Call& statement) override;
                void visit(IfStatement& statement) override;
                void visit(Return& statement) override;
                void visit(VariableAssignment& statement) override;

            private:
                const ParsedCommandLineArgs* _args;
                ScopeStack _scope_stack;
                Module* _module;
                analysis::FunctionMap _functions;
                std::stack<int> _func_index_stack;
                std::vector<errors::Error> _errors;
                Regs _register_stack;

                // Mapping from variable names to their registers
                std::unordered_map<std::string, Reg> _var_to_reg;

            private:
                void trace_kir(const std::string& name, const std::string& msg = "");
                Function& current_function();
                Module& current_module();
                void enter_function(kore::Function& func);
                void exit_function(kore::Function& func);
                void add_kir_function(kore::Function* function);
                Regs visit_function_arguments(class Call& call);
                Regs allocate_function_return_registers(class Call& call);

                Reg visit_expression(Expression* expr);
                void check_register_state(Identifier& expr, Reg reg);
                void push_register(Reg reg);
                void push_register(Reg reg, const Type* type);
                Reg pop_register();
        };
    }
}

#endif // KORE_KIR_LOWERING_PASS_HPP
