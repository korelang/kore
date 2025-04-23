#include "analysis/function_name_visitor.hpp"
#include "ast/expressions/integer_expression.hpp"
#include "ast/statements/statements.hpp"
#include "bin/korec/options.hpp"
#include "logging/logging.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/kir_lowering_pass.hpp"
#include "targets/bytecode/register.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"
#include "types/function_type.hpp"

#include <cassert>

namespace kore {
    namespace kir {
        KirLoweringPass::KirLoweringPass(const ParsedCommandLineArgs& args) : _args(&args) {}

        Module KirLoweringPass::lower(const Ast& ast) {
            kore::analysis::FunctionNameVisitor function_name_visitor{};

            _functions = function_name_visitor.collect_functions(ast);

            Module module(0, ast.path());

            _module = &module;

            // Add main function
            add_kir_function(nullptr);

            for (auto const& statement : ast) {
                statement->accept(*this);
            }

            // Emit a return instruction at the end of the main function so we
            // pop its call frame
            current_function().emit_return();

            _func_index_stack.pop();
            assert(_func_index_stack.size() == 0);

            return module;
        }

        void KirLoweringPass::visit(ArrayExpression& expr) {
            trace_kir("array expression", std::to_string(expr.size()));

            auto function = current_function();
            auto array_reg = function.emit_allocate_array(expr.size());

            // Visit its expression in the array and store it in the array
            for (int idx = 0; idx < expr.size(); ++idx) {
                auto element_expr = expr[idx];
                Reg element_reg = visit_expression(element_expr);

                // Load the index into a register
                Reg index_reg = function.emit_load(Bytecode::Cload, idx);

                function.emit_reg3(Bytecode::ArraySet, array_reg, element_reg, index_reg);
            }

            push_register(array_reg, expr.type());
        }

        void KirLoweringPass::visit(IndexExpression& expr) {
            auto function = current_function();
            Reg reg = function.allocate_register();
            Reg indexed_reg = visit_expression(expr.expr());
            Reg index_expr_reg = visit_expression(expr.index_expr());

            // Emit code based on the type of the indexed expression
            switch (expr.expr()->type()->category()) {
                case kore::TypeCategory::Array: {
                    function.emit_reg3(Bytecode::ArrayGet, indexed_reg, index_expr_reg, reg);
                    break;
                }

                default: {
                    // TODO:
                    break;
                }
            }

            push_register(reg);
        }

        void KirLoweringPass::visit(IndexExpression& expr, ValueContext context) {
            if (context == ValueContext::RValue) {
                visit(expr);
                return;
            }

            Reg reg = pop_register();
            Reg indexed_reg = visit_expression(expr.expr());
            Reg index_expr_reg = visit_expression(expr.index_expr());

            // Emit code based on the type of the indexed expression
            switch (expr.expr()->type()->category()) {
                case kore::TypeCategory::Array: {
                    current_function().emit_reg3(Bytecode::ArraySet, indexed_reg, index_expr_reg, reg);
                    break;
                }

                default: {
                    // TODO:
                    break;
                }
            }

            push_register(indexed_reg);
        }

        void KirLoweringPass::visit(BoolExpression& expr) {
            trace_kir("bool", expr.value());
            push_register(current_function().emit_load(Bytecode::LoadBool, expr, expr.bool_value() ? 1 : 0), expr.type());
        }

        void KirLoweringPass::visit(IntegerExpression& expr) {
            trace_kir(expr.type()->name().c_str(), std::to_string(expr.value()));

            int index = current_module().constant_table().add(expr.value());

            push_register(
                current_function().emit_load(Bytecode::Cload, expr, index),
                expr.type()
            );
        }

        void KirLoweringPass::visit(FloatExpression& expr) {
            trace_kir(expr.type()->name().c_str(), std::to_string(expr.value()));

            int index = current_module().constant_table().add(expr.value());

            push_register(
                current_function().emit_load(Bytecode::Cload, expr, index),
                expr.type()
            );
        }

        void KirLoweringPass::visit(BinaryExpression& expr) {
            trace_kir("binop", expr.op_string());

            Reg reg_left = visit_expression(expr.left());
            Reg reg_right = visit_expression(expr.right());

            push_register(
                current_function().emit_binop(expr, reg_left, reg_right),
                expr.type()
            );
        }

        void KirLoweringPass::visit(Identifier& expr) {
            trace_kir("identifier", expr.name());

            auto entry = _scope_stack.find(expr.name());

            if (entry->is_global_scope()) {
                auto reg = current_function().emit_load(Bytecode::Gload, expr, entry->reg);
                push_register(reg, expr.type());
            } else {
                check_register_state(expr, entry->reg);
                push_register(entry->reg, expr.type());
            }
        }

        void KirLoweringPass::visit(Identifier& expr, ValueContext context) {
            if (context == ValueContext::RValue) {
                visit(expr);
                return;
            }

            trace_kir("identifier (lvalue)", expr.name());

            // Get the register generated by the right-hand side expression
            Reg rhs_reg = pop_register();

            Reg dst;
            auto& func = current_function();
            auto entry = _scope_stack.find_inner(expr.name());

            if (!entry) {
                // NOTE: Is it always ok to directly use the rhs register?
                _scope_stack.insert(&expr, rhs_reg);
            } else {
                dst = entry->reg;

                // Free the register so whatever was there is freed
                func.free_register(dst);
            }
        }

        void KirLoweringPass::visit(VariableAssignment& assignment) {
            trace_kir("assignment");

            // Generate code for the right-hand side expression and push the resulting
            // register for the left-hand side
            for (int idx = 0; idx < assignment.rhs_count(); ++idx) {
                push_register(visit_expression(assignment.rhs(idx)));
            }

            // Generate code for the left-hand side expression. Right-hand side
            // registers were pushed in LIFO order so start from the end
            for (int idx = assignment.lhs_count() - 1; idx >= 0; --idx) {
                assignment.lhs(idx)->accept(*this, ValueContext::LValue);
            }
        }

        void KirLoweringPass::visit(IfStatement& ifstatement) {
            const std::string msg = std::to_string(ifstatement.branch_count()) + " branch(es)";
            trace_kir("if", msg);

            auto& func = current_function();
            auto& graph = func.graph();
            auto after_block_id = graph.add_block();

            BlockId before_block_id = graph.current_block().id;
            BlockId first_condition_block_id = BasicBlock::InvalidBlockId;

            // Save the previous block ids and condition register so
            // we can properly link up blocks when we generate code
            // for the subsequent blocks
            auto prev_block_id = BasicBlock::InvalidBlockId;
            auto condition_block_id = BasicBlock::InvalidBlockId;
            Reg prev_cond_reg = INVALID_REGISTER;

            for (auto& branch : ifstatement) {
                auto condition = branch->condition();
                Reg cond_reg = INVALID_REGISTER;

                if (condition) {
                    condition_block_id = graph.add_block_as_current();
                    cond_reg = visit_expression(condition);

                    if (first_condition_block_id == BasicBlock::InvalidBlockId) {
                        // Connect the block before the if statement to the
                        // first condition block
                        graph.add_edge(before_block_id, condition_block_id);
                    }
                }

                auto branch_id = graph.add_block_as_current();

                if (condition) {
                    graph.add_edge(condition_block_id, branch_id);
                }

                // Generate code for the branch
                for (auto& statement : *branch) {
                    statement->accept(*this);
                }

                // TODO: This should actually only be generated if this is not
                // the last branch in the if statement
                if (condition) {
                    // Generate an unconditional jump from the end of this
                    // branch (which was taken) to the after block
                    func.emit_unconditional_jump(after_block_id);
                }

                graph.add_edge(branch_id, after_block_id);

                if (prev_block_id != BasicBlock::InvalidBlockId) {
                    auto target_id = condition ? condition_block_id : branch_id;

                    graph.add_edge(prev_block_id, target_id);

                    // Generate a condition jump instruction at the end of the last
                    // condition block to this condition
                    graph.set_current_block(prev_block_id);

                    func.emit_conditional_jump(Bytecode::JumpIfNot, prev_cond_reg, target_id);
                }

                prev_block_id = condition_block_id;
                prev_cond_reg = cond_reg;
            }

            graph.set_current_block(after_block_id);
        }

        void KirLoweringPass::visit(kore::Function& func) {
            trace_kir("function", func.name());

            // Enter a new function scope and add all function arguments to
            // that scope
            enter_function(func);

            for (auto& statement : func) {
                statement->accept(*this);
            }

            exit_function();
        }

        Regs KirLoweringPass::visit_function_arguments(class Call& call) {
            Regs arg_registers;

            for (int idx = 0; idx < call.arg_count(); ++idx) {
                arg_registers.push_back(visit_expression(call.arg(idx)));
            }

            return arg_registers;
        }

        void KirLoweringPass::visit(class Call& call) {
            auto& func = current_function();
            Regs arg_registers = visit_function_arguments(call);
            auto opcode = Bytecode::LoadFunction;
            int func_index = -1;
            int return_register_count = 0;

            // Check if this is a built-in function
            auto builtin_function = vm::get_builtin_function_by_name(call.name());

            if (builtin_function) {
                trace_kir("builtin call", call.name());
                opcode = Bytecode::LoadBuiltin;
                func_index = builtin_function->index;
                return_register_count = builtin_function->type->return_arity();
            } else {
                trace_kir("call", call.name());

                auto user_func = _functions[call.name()];
                func_index = user_func.func_index;
                return_register_count = user_func.func->type()->return_arity();
            }

            Regs return_registers = func.allocate_registers(return_register_count);

            func.emit_call(
                func.emit_load_function(func_index, opcode),
                arg_registers,
                return_registers
            );

            // Push all return registers
            for (auto reg : return_registers) {
                push_register(reg);
            }
        }

        void KirLoweringPass::visit(Return& ret) {
            trace_kir("return");
            auto& func = current_function();

            // If the return statement returns an expression, generate code
            // for it, then get its register
            if (ret.expr_count() > 0) {
                std::vector<Reg> regs;

                for (auto& expr : ret) {
                    regs.push_back(visit_expression(expr.get()));
                }

                func.emit_return(regs);

                for (auto reg : regs) {
                    func.set_register_state(reg, RegisterState::Moved);
                }
            } else {
                // Otherwise return zero registers
                func.emit_return();
            }

            func.free_registers();
        }

        void KirLoweringPass::trace_kir(const std::string& name, const std::string& msg) {
            if (_args && _args->trace == TraceOption::Kir) {
                std::string message = name;

                if (!msg.empty()) {
                    message += " (" + msg + ")";
                }

                debug_group("kir", "%s", message.c_str());
            }
        }

        Function& KirLoweringPass::current_function() {
            return (*_module)[_func_index_stack.top()];
        }

        Module& KirLoweringPass::current_module() {
            return *_module;
        }

        void KirLoweringPass::enter_function(kore::Function& func) {
            add_kir_function(&func);
            _scope_stack.enter_function_scope(&func);

            for (int i = 0; i < func.arity(); ++i) {
                auto parameter = func.parameter(i);
                _scope_stack.insert(parameter, current_function().allocate_register());
            }
        }

        void KirLoweringPass::exit_function() {
            _func_index_stack.pop();
            _scope_stack.leave_function_scope();
        }

        void KirLoweringPass::add_kir_function(kore::Function* function) {
            _func_index_stack.push(_module->add_function(function));

            auto& graph = current_function().graph();

            // Add the start block
            graph.add_block(BasicBlock::StartBlockId);

            // Add the end block
            // TODO: Connect this as the function's AST is traversed
            graph.add_block(BasicBlock::EndBlockId);

            // Add the first block of this function and connect it to the source block
            graph.add_edge(BasicBlock::StartBlockId, graph.add_block_as_current());
        }

        Reg KirLoweringPass::visit_expression(Expression* expr) {
            expr->accept(*this);

            return pop_register();
        }

        void KirLoweringPass::check_register_state(Identifier& expr, Reg reg) {
            auto& func = current_function();

            if (func.register_state(reg) != RegisterState::Available) {
                // TODO: Move push_error into a separate class or Error class
                /*push_error(*/errors::kir::moved_variable(expr);/*);*/
            }
        }

        void KirLoweringPass::push_register(Reg reg) {
            _register_stack.push_back(reg);
        }

        void KirLoweringPass::push_register(Reg reg, const Type* type) {
            _register_stack.push_back(reg);
            current_function().set_register_type(reg, type);
        }

        Reg KirLoweringPass::pop_register() {
            if (_register_stack.empty()) {
                throw std::runtime_error("No register was pushed when popping");
            }

            auto reg = _register_stack.back();
            _register_stack.pop_back();

            return reg;
        }
    }
}
