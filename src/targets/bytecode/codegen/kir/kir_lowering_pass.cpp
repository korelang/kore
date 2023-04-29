#include "ast/expressions/integer_expression.hpp"
#include "ast/statements/statements.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/kir_lowering_pass.hpp"

// TODO: Can this be moved into the ast visitor class instead?
#if defined(KORE_DEBUG_KIR_LOWERING_PASS) || defined(KORE_DEBUG)
    #include "logging/logging.hpp"

    #define KORE_DEBUG_KIR_LOWERING_PASS_LOG(prefix, msg) {\
        debug_group("compiler:kir", "%s (%s)", prefix.c_str(), msg.c_str());\
    }
#else
    #define KORE_DEBUG_KIR_LOWERING_PASS_LOG(prefix, msg) {}
#endif

namespace kore {
    namespace kir {
        KirLoweringPass::KirLoweringPass() {}

        Module KirLoweringPass::lower(const Ast& ast) {
            // TODO
            Module module(0, "");

            _func_index = 0;
            _module = &module;
            _module->add_function(_func_index++);

            auto graph = current_function().graph();

            // Add the source block
            graph.set_current_block(graph.add_block(0));

            // Add the first block of this function and connect it to the source block
            graph.add_edge(0, graph.add_block());

            for (auto const& statement : ast) {
                statement->accept_visit_only(*this);
            }

            return module;
        }

        void KirLoweringPass::visit_only(ArrayExpression& expr) {
            KORE_DEBUG_KIR_LOWERING_PASS_LOG("array", expr.value());

            std::vector<Reg> element_regs;

            // TODO: This needs to be limited to a threshold for very large
            // arrays and perhaps use a loop instead
            for (int idx = 0; idx < expr.size(); ++idx) {
                element_regs.push_back(visit_expression(expr[idx]));
            }

            current_function().allocate_array(expr, element_regs);
        }

        void KirLoweringPass::visit_only(BoolExpression& expr) {
            KORE_DEBUG_KIR_LOWERING_PASS_LOG("bool", expr.value());
            current_function().load_constant(expr);
        }

        /* void KirLoweringPass::visit_only(CharExpression& expr) { */
        /*     KORE_DEBUG_KIR_LOWERING_PASS_LOG("char", expr.value()); */
        /*     current_function().load_constant(expr); */
        /* } */

        void KirLoweringPass::visit_only(IntegerExpression& expr) {
            /* int num_bits = expr.type()->category() == TypeCategory::Integer32 ? 32 : 64; */

            KORE_DEBUG_KIR_LOWERING_PASS_LOG(
                "i" + std::to_string(num_bits),
                expr.value()
            );

            current_function().load_constant(expr);
        }

        void KirLoweringPass::visit_only(FloatExpression& expr) {
            /* int num_bits = expr.type()->category() == TypeCategory::Float32 ? 32 : 64; */

            KORE_DEBUG_KIR_LOWERING_PASS_LOG(
                "f" + std::to_string(num_bits),
                expr.value()
            );

            current_function().load_constant(expr);
        }

        void KirLoweringPass::visit_only(BinaryExpression& expr) {
            Reg reg_left = visit_expression(expr.left());
            Reg reg_right = visit_expression(expr.right());

            current_function().binop(expr, reg_left, reg_right);
        }

        void KirLoweringPass::visit_only(Identifier& expr) {
            KORE_DEBUG_KIR_LOWERING_PASS_LOG("identifier", expr.value());

            auto entry = _scope_stack.find(expr.name());

            if (entry->is_global_scope()) {
                auto reg = current_function().load_global(expr, entry->reg);
                push_register(reg);
            } else {
                check_register_state(expr, entry->reg);
                push_register(entry->reg);
            }
        }

        void KirLoweringPass::visit_only(VariableAssignment& statement) {
            auto func = current_function();
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

            func.set_register_state(dst, RegisterState::Available);
            func.move(statement, dst, src);

            // If we reference a variable which is not a value type (e.g. an
            // array), increase its reference count
            if (rhs->expr_type() == ExpressionType::Identifier) {
                if (!rhs->type()->is_value_type()) {
                    func.refinc(src);
                }
            }
        }

        void KirLoweringPass::visit_only(IfStatement& ifstatement) {
            auto func = current_function();
            auto graph = func.graph();
            auto after = graph.add_block();
            auto prev_block = BasicBlock::InvalidBlockId;
            auto prev_true_branch = BasicBlock::InvalidBlockId;

            for (auto& branch : ifstatement) {
                auto condition_block = BasicBlock::InvalidBlockId;
                auto condition = branch->condition();
                Reg cond_reg;

                if (condition) {
                    condition_block = graph.add_block_as_current();
                    cond_reg = visit_expression(condition);
                }

                auto true_branch = graph.add_block_as_current();

                graph.add_edge(condition_block, true_branch);
                graph.add_edge(true_branch, after);

                for (auto& statement : *branch) {
                    statement->accept_visit_only(*this);
                }

                if (prev_block != BasicBlock::InvalidBlockId) {
                    if (condition) {
                        graph.add_edge(prev_block, condition_block);

                        // Connect the last branch to this one
                        graph.set_current_block(prev_block);
                        func.branch(cond_reg, prev_true_branch, condition_block);
                    } else {
                        // Else block
                        graph.add_edge(prev_block, true_branch);
                    }
                } else {
                    prev_block = condition_block;
                    prev_true_branch = true_branch;
                }
            }

            graph.set_current_block(after);
        }

        void KirLoweringPass::visit_only(kore::Function& func) {
            KORE_DEBUG_KIR_LOWERING_PASS_LOG("function", func.name());

            // Enter a new function scope and add all function arguments to
            // that scope
            enter_function(func);

            for (auto& statement : func) {
                statement->accept_visit_only(*this);
            }

            exit_function();
        }

        void KirLoweringPass::visit_only(Return& ret) {
            KORE_DEBUG_KIR_LOWERING_PASS_LOG("return", std::string())
            auto func = current_function();

            // If the return statement returns an expression, generate code
            // for it, then get its register
            if (ret.expr()) {
                auto reg = visit_expression(ret.expr());
                func._return(reg);
                func.set_register_state(reg, RegisterState::Moved);
            } else {
                // Otherwise return zero registers
                func._return();
            }

            func.free_registers();
        }

        Function& KirLoweringPass::current_function() {
            return (*_module)[_func_index];
        }

        void KirLoweringPass::enter_function(kore::Function& func) {
            _module->add_function(func);

            ++_func_index;
            _scope_stack.enter_function_scope(&func);

            for (int i = 0; i < func.arity(); ++i) {
                auto parameter = func.parameter(i);
                _scope_stack.insert(parameter, current_function().allocate_register());
            }
        }

        void KirLoweringPass::exit_function() {
            --_func_index;
            _scope_stack.leave_function_scope();
        }

        Reg KirLoweringPass::visit_expression(Expression* expr) {
            expr->accept_visit_only(*this);

            auto reg = _register_stack.back();
            _register_stack.pop_back();

            return reg;
        }

        void KirLoweringPass::check_register_state(Identifier& expr, Reg reg) {
            auto func = current_function();

            if (func.register_state(reg) != RegisterState::Available) {
                // TODO: Move push_error into a separate class or Error class
                /*push_error(*/errors::kir::moved_variable(expr);/*);*/
            }
        }

        void KirLoweringPass::push_register(Reg reg) {
            _register_stack.push_back(reg);
        }
    }
}

#undef KORE_DEBUG_KIR_LOWERING_PASS_LOG
