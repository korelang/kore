#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "types/function_type.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

#if defined(KORE_DEBUG_TYPEINFERRER) || defined(KORE_DEBUG)
    #include <iostream>

    #define KORE_DEBUG_TYPEINFERRER_LOG(prefix) {\
        std::cerr << "[type inferrer] " << prefix << std::endl;\
    }

    #define KORE_DEBUG_TYPEINFERRER_LOG_TYPE(prefix, type) {\
        std::cerr\
            << "[type inferrer] " << prefix\
            << ": " << type->name() << std::endl;\
    }
#else
    #define KORE_DEBUG_TYPEINFERRER_LOG(prefix)
    #define KORE_DEBUG_TYPEINFERRER_LOG_TYPE(prefix, type)
#endif

namespace kore {
    TypeInferrer::TypeInferrer() {}

    TypeInferrer::~TypeInferrer() {}

    void TypeInferrer::infer(Ast& ast) {
        for (auto& statement : ast) {
            statement->accept_visit_only(*this);
        }
    }

    void TypeInferrer::visit(BinaryExpression& expr) {
        expr.left()->accept_visit_only(*this);
        expr.right()->accept_visit_only(*this);
        expr.set_type(expr.left()->type()->unify(expr.right()->type()));

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("binop", expr.type())
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

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("call " + call.name(), call.type())
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

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE(
            "identifier " + expr.name(),
            expr.type()
        )
    }

    void TypeInferrer::visit(UnaryExpression& expr) {
        expr.expr()->accept_visit_only(*this);
        expr.set_type(expr.expr()->type());

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("unary expression", expr.type())
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

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("assignment", statement.type())
    }

    /* void TypeInferrer::visit(Function& statement) { */
    /*     auto last = statement->last_statement(); */

    /*     if (!last->type()->is_unknown()) { */
    /*         statement->set_return_type(last->type()); */
    /*     } */
    /* } */

    void TypeInferrer::visit(Function& func) {
        KORE_DEBUG_TYPEINFERRER_LOG("function")
        UNUSED_PARAM(func);

        // Enter a new function scope and add all function
        // arguments to that scope
        _scope_stack.enter_function_scope();

        for (int i = 0; i < func.arity(); ++i) {
            auto parameter = func.parameter(i);
            _scope_stack.insert(parameter);
        }

        for (auto& statement : func) {
            statement->accept_visit_only(*this);
        }

        _scope_stack.leave();

        // After leaving the function scope, bind the function type
        // to the function name (not necessarily in the top-level scope
        // since we want to support nested functions)
        _scope_stack.insert(func.identifier());
    }

    void TypeInferrer::visit(Return& ret) {
        KORE_DEBUG_TYPEINFERRER_LOG("return")

        if (ret.expr()) {
            ret.expr()->accept_visit_only(*this);
        }
    }

    void TypeInferrer::visit(Branch& branch) {
        KORE_DEBUG_TYPEINFERRER_LOG("branch")
        UNUSED_PARAM(branch);

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

#undef KORE_DEBUG_TYPEINFERRER_LOG
#undef KORE_DEBUG_TYPEINFERRER_LOG_TYPE
