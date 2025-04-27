#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "bin/korec/options.hpp"
#include "logging/logging.hpp"
#include "types/array_type.hpp"
#include "types/function_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    TypeInferrer::TypeInferrer(const ParsedCommandLineArgs& args) : _args(args) {}

    TypeInferrer::~TypeInferrer() {}

    void TypeInferrer::infer(Ast& ast) {
        for (auto& statement : ast) {
            statement->accept(*this);
        }
    }

    void TypeInferrer::trace_type_inference(const Expression& expr) {
        if (_args.trace == TraceOption::TypeInference) {
            debug_group(
                _debug_group,
                "inferred type %s for %s expression at %s",
                expr.type()->name().c_str(),
                expr.type_name().c_str(),
                expr.location().format().c_str()
            );
        }
    }

    void TypeInferrer::trace_type_inference(
        const Statement& statement,
        const Type* type
    ) {
        if (_args.trace == TraceOption::TypeInference) {
            if (type) {
                debug_group(
                    _debug_group,
                    "statement %s with type %s at %s",
                    statement.name().c_str(),
                    type->name().c_str(),
                    statement.location().format().c_str()

                );
            } else {
                debug_group(_debug_group, "%s", statement.name().c_str());
            }
        }
    }

    void TypeInferrer::visit(ArrayExpression& array) {
        if (array.size() == 0) {
            array.set_type(Type::unknown());
            return;
        }

        // Infer the types of all array elements
        for (int idx = 0; idx < array.size(); ++idx) {
            array[idx]->accept(*this);
        }

        const Type* inferred_element_type = array[0]->type();

        // We infer the element type of an array expression (like [1, 2, 3]) by
        // unifying all element types
        for (int idx = 1; idx < array.size(); ++idx) {
            inferred_element_type = inferred_element_type->unify(array[idx]->type());
        }

        array.set_type(Type::make_array_type(inferred_element_type));
        trace_type_inference(array);
    }

    void TypeInferrer::visit(IndexExpression& index_expr) {
        index_expr.index_expr()->accept(*this);

        switch (index_expr.type()->category()) {
            case TypeCategory::Array: {
                index_expr.set_type(
                    static_cast<const ArrayType*>(index_expr.type())->element_type()
                );
                break;
            }

            default: {
                break;
            }
        }
    }

    void TypeInferrer::visit(IndexExpression& array_index, ValueContext context) {
        UNUSED_PARAM(array_index);
        UNUSED_PARAM(context);
    }

    void TypeInferrer::visit(BinaryExpression& expr) {
        expr.left()->accept(*this);
        expr.right()->accept(*this);
        expr.set_type(expr.left()->type()->unify(expr.right()->type()));

        // TODO: Should expression have names or a ostream operator instead?
        trace_type_inference(expr);
    }

    void TypeInferrer::visit(class Call& call) {
        auto entry = _scope_stack.find(call.name());

        if (entry) {
            auto type = entry->identifier->type();

            if (type->is_function()) {
                auto func_type = static_cast<const FunctionType*>(type);

                // The resulting type of calling a function is its return type
                call.set_type(func_type);
            } else {
                // Otherwise, the type of the call is unknown
                call.set_type(Type::unknown());
            }
        } else {
            call.set_type(Type::unknown());
        }

        trace_type_inference(call);
    }

    void TypeInferrer::visit(Identifier& expr) {
        auto entry = _scope_stack.find(expr.name());

        if (entry) {
            expr.set_type(entry->identifier->type());
        } else {
            auto declared_type = expr.declared_type();

            if (!declared_type->is_unknown()) {
                expr.set_type(declared_type);
            }

            _scope_stack.insert(&expr);
        }

        trace_type_inference(expr);
    }

    void TypeInferrer::visit(Identifier& expr, ValueContext context) {
        UNUSED_PARAM(context);
        visit(expr);
    }

    void TypeInferrer::visit(UnaryExpression& expr) {
        expr.expr()->accept(*this);
        expr.set_type(expr.expr()->type());

        trace_type_inference(expr);
    }

    void TypeInferrer::visit(VariableAssignment& assignment) {
        // Visit the right-hand side expressions
        for (int idx = 0; idx < assignment.rhs_count(); ++idx) {
            assignment.rhs(idx)->accept(*this);
        }

        for (int idx = 0; idx < assignment.lhs_count(); ++idx) {
            auto lhs_expr = assignment.lhs(idx);
            lhs_expr->accept(*this, ValueContext::LValue);

            if (lhs_expr->type()->is_unknown()) {
                // If the type is still unknown, it is being assigned for the first time
                // here and it has no declared type so use the type of the right-hand side
                // value
                lhs_expr->set_type(assignment.rhs_type(idx));
            }

            trace_type_inference(*lhs_expr);
        }
    }

    /* void TypeInferrer::visit(Function& statement) { */
    /*     auto last = statement->last_statement(); */

    /*     if (!last->type()->is_unknown()) { */
    /*         statement->set_return_type(last->type()); */
    /*     } */
    /* } */

    void TypeInferrer::visit(Function& func) {
        trace_type_inference(func, func.type());

        // Enter a new function scope and add all function
        // arguments to that scope
        _scope_stack.enter_function_scope(&func);

        // TODO: Move into enter_function_scope?
        for (int i = 0; i < func.arity(); ++i) {
            auto parameter = func.parameter(i);
            _scope_stack.insert(parameter);
        }

        for (auto& statement : func) {
            statement->accept(*this);
        }

        _scope_stack.leave_function_scope();

        // After leaving the function scope, bind the function type
        // to the function name (not necessarily in the top-level scope
        // since we want to support nested functions)
        _scope_stack.insert(func.identifier());
    }

    void TypeInferrer::visit(Return& ret) {
        trace_type_inference(ret);

        if (ret.expr_count() > 0) {
            for (int idx = 0; idx < ret.expr_count(); ++idx) {
                ret.get_expr(idx)->accept(*this);
            }
        }
    }

    void TypeInferrer::visit(Branch& branch) {
        trace_type_inference(branch);

        _scope_stack.enter();

        if (branch.condition()) {
            branch.condition()->accept(*this);
        }

        for (auto& statement : branch) {
            statement->accept(*this);
        }

        _scope_stack.leave();
    }

    std::string TypeInferrer::_debug_group = "type_inference";
}
