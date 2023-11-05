#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "bin/korec/options.hpp"
#include "logging/logging.hpp"
#include "types/function_type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    TypeInferrer::TypeInferrer(const ParsedCommandLineArgs& args) : _args(&args) {}

    TypeInferrer::~TypeInferrer() {}

    void TypeInferrer::infer(Ast& ast) {
        for (auto& statement : ast) {
            statement->accept_visit_only(*this);
        }
    }

    void TypeInferrer::trace_type_inference(
        const std::string& name,
        const Type* type
    ) {
        if (_args && _args->trace == TraceOption::TypeInference) {
            const std::string group = "type_inference";

            if (type) {
                debug_group(group, "%s (type: %s)", name.c_str(), type->name().c_str());
            } else {
                debug_group(group, "%s", name.c_str());
            }
        }
    }

    void TypeInferrer::visit(BinaryExpression& expr) {
        expr.left()->accept_visit_only(*this);
        expr.right()->accept_visit_only(*this);
        expr.set_type(expr.left()->type()->unify(expr.right()->type()));

        // TODO: Should expression have names or a ostream operator instead?
        trace_type_inference("binop", expr.type());
    }

    void TypeInferrer::visit(class Call& call) {
        auto entry = _scope_stack.find(call.name());

        if (entry) {
            auto type = entry->identifier->type();

            if (type->is_function()) {
                auto func_type = static_cast<const FunctionType*>(entry->identifier->type());

                // The resulting type of calling a function is its return type
                call.set_type(func_type->return_type());
            } else {
                // Otherwise, the type of the call is unknown
                call.set_type(Type::unknown());
            }
        } else {
            call.set_type(Type::unknown());
        }

        trace_type_inference("call to " + call.name(), call.type());
    }

    void TypeInferrer::visit(Identifier& expr) {
        auto entry = _scope_stack.find(expr.name());

        // If no identifier was found, this is an undefined variable which is
        // caught by the type checker
        if (entry) {
            expr.set_type(entry->identifier->type());
        } else {
            expr.set_type(Type::unknown());
        }

        trace_type_inference("identifier " + expr.name(), expr.type());
    }

    void TypeInferrer::visit(UnaryExpression& expr) {
        expr.expr()->accept_visit_only(*this);
        expr.set_type(expr.expr()->type());

        trace_type_inference("unary expression", expr.type());
    }

    void TypeInferrer::visit(VariableAssignment& statement) {
        // Do not infer types for variable assignments with an explicit type
        if (!statement.type()->is_unknown()) {
            return;
        }

        statement.expression()->accept_visit_only(*this);
        auto expr_type = statement.expression()->type();

        // Set the type of the identifier/variable and save it
        // in the symbol table
        statement.set_type(expr_type);
        _scope_stack.insert(statement.identifier());

        trace_type_inference("assignment", statement.type());
    }

    /* void TypeInferrer::visit(Function& statement) { */
    /*     auto last = statement->last_statement(); */

    /*     if (!last->type()->is_unknown()) { */
    /*         statement->set_return_type(last->type()); */
    /*     } */
    /* } */

    void TypeInferrer::visit(Function& func) {
        trace_type_inference("function " + func.name(), func.type());

        // Enter a new function scope and add all function
        // arguments to that scope
        _scope_stack.enter_function_scope(&func);

        // TODO: Move into enter_function_scope?
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

    void TypeInferrer::visit(Return& ret) {
        trace_type_inference("return");

        if (ret.expr()) {
            ret.expr()->accept_visit_only(*this);
        }
    }

    void TypeInferrer::visit(Branch& branch) {
        trace_type_inference("branch");

        _scope_stack.enter();

        if (branch.condition()) {
            branch.condition()->accept_visit_only(*this);
        }

        for (auto& statement : branch) {
            statement->accept_visit_only(*this);
        }

        _scope_stack.leave();
    }
}
