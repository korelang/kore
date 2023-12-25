#include "analysis/function_name_visitor.hpp"
#include "ast/expressions/integer_expression.hpp"
#include "ast/statements/statements.hpp"
#include "bin/korec/options.hpp"
#include "logging/logging.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/kir_lowering_pass.hpp"
#include "targets/bytecode/register.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"
#include "utils/unused_parameter.hpp"

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
                statement->accept_visit_only(*this);
            }

            // Emit a return instruction at the end of the main function so we
            // pop its call frame
            current_function().emit_return();

            _func_index_stack.pop();
            assert(_func_index_stack.size() == 0);

            return module;
        }

        void KirLoweringPass::visit(ArrayExpression& expr) {
            trace_kir("array", std::to_string(expr.size()));

            /* Regs element_regs; */

            /* // TODO: This needs to be limited to a threshold for very large */
            /* // arrays and perhaps use a loop instead */
            /* for (int idx = 0; idx < expr.size(); ++idx) { */
            /*     element_regs.push_back(visit_expression(expr[idx])); */
            /* } */

            push_register(
                current_function().emit_allocate_array(expr),//, element_regs),
                expr.type()
            );
        }

        void KirLoweringPass::visit(BoolExpression& expr) {
            trace_kir("bool", expr.value());
            push_register(current_function().emit_load(Bytecode::LoadBool, expr, expr.bool_value() ? 1 : 0), expr.type());
        }

        /* void KirLoweringPass::visit(CharExpression& expr) { */
        /*     trace_kir("char", expr.value()); */
        /*     current_function().load_constant(expr); */
        /* } */

        void KirLoweringPass::visit(IntegerExpression& expr) {
            trace_kir(expr.type()->name().c_str(), std::to_string(expr.value()));

            int index = current_module().constant_table().add(expr.value());

            push_register(current_function().emit_load(Bytecode::Cload, expr, index), expr.type());
        }

        void KirLoweringPass::visit(FloatExpression& expr) {
            trace_kir(expr.type()->name().c_str(), std::to_string(expr.value()));

            int index = current_module().constant_table().add(expr.value());

            push_register(current_function().emit_load(Bytecode::Cload, expr, index), expr.type());
        }

        /* void KirLoweringPass::visit(StringExpression& expr) { */
        /*     trace_kir("str", expr.value()); */

        /*     push_register(current_object()->allocate_register()); */
        /* } */

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

        void KirLoweringPass::visit(VariableAssignment& statement) {
            trace_kir("assignment", statement.identifier()->name());

            auto& func = current_function();
            auto entry = _scope_stack.find_inner(statement.identifier()->name());
            auto rhs = statement.expression();
            Reg src = visit_expression(rhs);
            Reg dst;

            if (!entry) {
                dst = func.allocate_register();
                _scope_stack.insert(statement.identifier(), dst);
            } else {
                dst = entry->reg;

                // Free the lhs register so whatever was there already is freed
                func.free_register(dst);
            }

            func.emit_move(dst, src);

            // If we reference a variable which is not a value type (e.g. an
            // array), increase its reference count
            if (rhs->expr_type() == ExpressionType::Identifier) {
                if (!rhs->type()->is_value_type()) {
                    func.emit_refinc(src);
                }
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
                    statement->accept_visit_only(*this);
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
                statement->accept_visit_only(*this);
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

        Regs KirLoweringPass::allocate_function_return_registers(class Call& call) {
            UNUSED_PARAM(call);

            // TODO: Support multiple return values. We need to know how many
            // values are being expected to be returned from the parser
            return { current_function().allocate_register() };
        }

        void KirLoweringPass::visit(class Call& call) {
            auto& func = current_function();
            Regs arg_registers = visit_function_arguments(call);
            Regs return_registers = allocate_function_return_registers(call);

            // Check if this is a built-in function
            auto builtin_function = vm::get_builtin_function_by_name(call.name());

            if (builtin_function) {
                trace_kir("builtin call", call.name());
                Regs builtin_return_registers;

                if (builtin_function->ret_count > 0) {
                    builtin_return_registers = return_registers;
                }

                func.emit_call(func.emit_load_function(builtin_function->index), arg_registers, builtin_return_registers);
            } else {
                trace_kir("call", call.name());

                int func_index = _functions[call.name()].first;
                func.emit_call(func.emit_load_function(func_index), arg_registers, return_registers);
            }

            // Push single destination (return) register
            push_register(return_registers[0]);
        }

        void KirLoweringPass::visit(Return& ret) {
            trace_kir("return");
            auto& func = current_function();

            // If the return statement returns an expression, generate code
            // for it, then get its register
            if (ret.expr()) {
                auto reg = visit_expression(ret.expr());
                func.emit_return(reg);
                func.set_register_state(reg, RegisterState::Moved);
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
            expr->accept_visit_only(*this);

            if (_register_stack.empty()) {
                throw std::runtime_error("No destination register was pushed when visiting expression");
            }

            auto reg = _register_stack.back();
            _register_stack.pop_back();

            return reg;
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
    }
}
