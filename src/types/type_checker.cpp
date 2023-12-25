#include "type_checker.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/branch.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "errors/errors.hpp"
/* #include "targets/bytecode/vm/builtins/builtins.hpp" */
#include "logging/logging.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"
#include "types/function_type.hpp"
#include "types/scope.hpp"

#include <sstream>

namespace kore {
    TypeChecker::TypeChecker(const ParsedCommandLineArgs& args) : _args(&args) {}

    TypeChecker::~TypeChecker() {}

    int TypeChecker::check(const Ast& ast) {
        _errors.clear();

        for (auto const& statement : ast) {
            statement->accept_visit_only(*this);
        }

        return _errors.size();
    }

    void TypeChecker::trace_type_checker(
        const std::string& name,
        const Type* type1,
        const Type* type2
    ) {
        if (_args && _args->trace == TraceOption::TypeCheck) {
            std::string msg = name;
            const std::string group = "type_check";

            if (type1) {
                msg += ": " + type1->name();
            }

            if (type2) {
                msg += ", " + type2->name();
            }

            debug_group(group, "%s", msg.c_str());
        }
    }

    std::vector<errors::Error> TypeChecker::errors() {
        return _errors;
    }

    void TypeChecker::push_error(errors::Error error) {
        if (_error_threshold != _NO_ERROR_THRESHOLD && static_cast<int>(_errors.size()) >= _error_threshold) {
            return;
        }

        _errors.emplace_back(error);
    }

    void TypeChecker::visit(Identifier& identifier) {
        auto entry = _scope_stack.find(identifier.name());

        if (entry) {
            trace_type_checker("identifier", entry->identifier->type());

            identifier.set_type(entry->identifier->type());
        } else {
            push_error(errors::typing::undefined_variable(identifier));
        }
    }

    void TypeChecker::visit(VariableAssignment& assignment) {
        assignment.expression()->accept_visit_only(*this);

        auto declared_type = assignment.declared_type();
        auto expr_type = assignment.expression()->type();

        trace_type_checker("assignment", declared_type, expr_type);

        // If the variable was not given an explicit type, rely on inferred
        // type instead
        if (!declared_type->is_unknown()) {
            if (declared_type->unify(expr_type)->is_unknown()) {
                push_error(errors::typing::cannot_assign(expr_type, declared_type, assignment.location()));
            }
        }

        auto identifier = assignment.identifier();
        auto entry = _scope_stack.find_inner(identifier->name());

        if (!entry) {
            if (_scope_stack.is_global_scope() && identifier->is_mutable()) {
                push_error(errors::typing::cannot_declare_mutable_global(*identifier, assignment.location()));
            } else {
                auto shadowed_entry = _scope_stack.find(identifier->name());

                if (shadowed_entry) {
                    auto shadowed_identifier = shadowed_entry->identifier;

                    if (shadowed_entry->is_global_scope()) {
                        push_error(errors::typing::cannot_assign_global_variable(
                            identifier,
                            shadowed_identifier,
                            assignment.location(),
                            shadowed_identifier->location()
                        ));
                    } else {
                        push_error(errors::typing::variable_shadows(
                            identifier,
                            shadowed_identifier,
                            assignment.location(),
                            shadowed_identifier->location()
                        ));
                    }
                }

                _scope_stack.insert(identifier);
            }
        } else {
            // Variable already exists in this scope, check that we are not
            // redeclaring a constant variable
            if (!entry->identifier->is_mutable()) {
                push_error(errors::typing::redeclaration_constant_variable(*identifier, assignment.location(), *entry->identifier));
            }
        }
    }

    void TypeChecker::visit(class Call& call) {
        trace_type_checker("call", call.type());

        // Find an scoped entry in the current or enclosing scopes for a
        // function definition
        auto entry = _scope_stack.find(call.name());

        if (!entry) {
            auto builtin_function = vm::get_builtin_function_by_name(call.name());

            if (!builtin_function) {
                push_error(errors::typing::unknown_call(call));
                return;
            }

            // TODO: Typecheck builtin function calls

            if (call.arg_count() != builtin_function->arity) {
                /* push_error(errors::typing::incorrect_arg_count( */
                /*     call, */
                /*     func_type */
                /* )); */

                return;
            }

            call.set_type(builtin_function->ret_types[0]);

            return;
        }

        auto type = entry->identifier->type();

        if (type->category() != TypeCategory::Function) {
            push_error(errors::typing::not_a_function(call, type));
            return;
        }

        auto func_type = static_cast<const FunctionType*>(type);

        if (call.arg_count() != func_type->arity()) {
            push_error(errors::typing::incorrect_arg_count(
                call,
                func_type
            ));

            return;
        }

        trace_type_checker("call", func_type);

        // TODO: Move into Call class
        for (int i = 0; i < call.arg_count(); ++i) {
            auto arg = call.arg(i);
            arg->accept_visit_only(*this);

            auto arg_type = arg->type();
            auto param_type = func_type->parameter(i)->type();
            auto unified_type = arg_type->unify(param_type);

            if (unified_type->is_unknown()) {
                push_error(errors::typing::incorrect_parameter_type(
                    arg,
                    arg_type,
                    param_type,
                    call,
                    i
                ));
            }
        }

        call.set_type(func_type->return_type());
    }

    void TypeChecker::visit(BinaryExpression& binexpr) {
        auto left = binexpr.left();
        auto right = binexpr.right();

        left->accept_visit_only(*this);
        right->accept_visit_only(*this);

        trace_type_checker("binop", left->type(), right->type());

        auto op = binexpr.op();
        auto left_type = left->type();
        auto right_type = right->type();
        auto left_type_compatible = compatible_binop_type(op, left_type);
        auto right_type_compatible = compatible_binop_type(op, right_type);

        // Check if types are compatible with the binary operation
        if (!left_type_compatible) {
            push_error(errors::typing::binop_operand(left_type, op, "left", left->location()));
        }

        if (!right_type_compatible) {
            push_error(errors::typing::binop_operand(right_type, op, "right", right->location()));
        }

        auto result_type = left_type->unify(right_type);

        // Handle type unification errors
        if (left_type_compatible && right_type_compatible) {
            if (result_type->is_unknown()) {
                push_error(
                    errors::typing::incompatible_binop(
                        left_type,
                        right_type,
                        op,
                        binexpr.location()
                    )
                );
            } else {
                binexpr.set_type(result_type);
            }
        }
    }

    void TypeChecker::visit(Branch& branch) {
        trace_type_checker("branch");

        _scope_stack.enter();

        if (branch.condition()) {
            branch.condition()->accept_visit_only(*this);
        }

        for (auto& statement : branch) {
            statement->accept_visit_only(*this);
        }

        _scope_stack.leave();
    }

    void TypeChecker::visit(IfStatement& statement) {
        for (auto& branch : statement) {
            auto condition = branch->condition();

            if (condition) {
                condition->accept_visit_only(*this);
            }

            for (auto& statement : *branch) {
                statement->accept_visit_only(*this);
            }
        }
    }

    void TypeChecker::visit(Function& func) {
        trace_type_checker("function", func.type());

        // Enter a new function scope and add all function
        // arguments to that scope
        _scope_stack.enter_function_scope(&func);

        for (int i = 0; i < func.arity(); ++i) {
            auto parameter = func.parameter(i);
            _scope_stack.insert(parameter);
        }

        for (auto& statement : func) {
            statement->accept_visit_only(*this);
        }

        _scope_stack.leave_function_scope();

        // After leaving the function scope, bind the function type
        // to the function name (not necessarily in the top-level scope
        // since we want to support nested functions)
        _scope_stack.insert(func.identifier());
    }

    void TypeChecker::visit(Return& ret) {
        trace_type_checker("return");

        auto func = _scope_stack.enclosing_function();

        if (ret.expr()) {
            auto expr = ret.expr();
            expr->accept_visit_only(*this);

            if (expr->type()->unify(func->return_type())->is_unknown()) {
                push_error(errors::typing::return_type_mismatch(func, expr->type(), ret.location()));
                return;
            }
        } else {
            if (!func->return_type()->is_void()) {
                push_error(errors::typing::void_return_from_nonvoid_function(func, ret.location()));
            }
        }
    }
}
