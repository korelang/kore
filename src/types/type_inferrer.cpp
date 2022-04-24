#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/binary_expression.hpp"
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
    TypeInferrer::TypeInferrer(ScopeStack& scope_stack)
        : _scope_stack(scope_stack) {}

    TypeInferrer::~TypeInferrer() {}

    void TypeInferrer::infer(Ast& ast) {
        for (auto& statement : ast) {
            statement->accept(this);
        }
    }

    bool TypeInferrer::precondition(VariableAssignment* statement) {
        KORE_DEBUG_TYPEINFERRER_LOG("pre assignment")

        // Do not infer types for a variable assignment with an explicit type
        return statement->type()->category() != TypeCategory::Unknown;
    }

    void TypeInferrer::visit(BinaryExpression* expr) {
        expr->set_type(expr->left()->type()->unify(expr->right()->type()));

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("binop", expr->type())
    }

    void TypeInferrer::visit(class Call* call) {
        auto entry = _scope_stack.find(call->name());

        if (entry) {
            call->set_type(entry->identifier->type());
        } else {
            call->set_type(Type::unknown());
        }

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("call " + call->name(), call->type())
    }

    void TypeInferrer::visit(Identifier* expr) {
        auto entry = _scope_stack.find(expr->name());

        // If no identifier was found, this is an undefined variable which is
        // caught by the type checker
        if (entry) {
            expr->set_type(entry->identifier->type());
        } else {
            expr->set_type(Type::unknown());
        }

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE(
            "identifier " + expr->name(),
            expr->type()
        )
    }

    void TypeInferrer::visit(UnaryExpression* expr) {
        expr->set_type(expr->expr()->type());

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("identifier", expr->type())
    }

    void TypeInferrer::visit(VariableAssignment* statement) {
        auto expr_type = statement->expression()->type();

        // Set the type of the identifier/variable and save it
        // in the symbol table
        statement->set_type(expr_type);
        _scope_stack.insert(statement->identifier());

        KORE_DEBUG_TYPEINFERRER_LOG_TYPE("assignment", statement->type())
    }

    /* void TypeInferrer::visit(Function* statement) { */
    /*     auto last = statement->last_statement(); */

    /*     if (!last->type()->is_unknown()) { */
    /*         statement->set_return_type(last->type()); */
    /*     } */
    /* } */

    bool TypeInferrer::precondition(Function* func) {
        UNUSED_PARAM(func);
        _scope_stack.enter_function_scope();
        return false;
    }

    bool TypeInferrer::postcondition(Function* func) {
        UNUSED_PARAM(func);
        _scope_stack.leave();

        // After leaving the function scope, bind the function type
        // to the function name (not necessarily in the top-level scope
        // since we want to support nested functions)
        _scope_stack.insert(func->identifier());

        return false;
    }

    bool TypeInferrer::precondition(Branch* branch) {
        KORE_DEBUG_TYPEINFERRER_LOG("pre branch")

        UNUSED_PARAM(branch);
        _scope_stack.enter();
        return false;
    }

    bool TypeInferrer::postcondition(Branch* branch) {
        KORE_DEBUG_TYPEINFERRER_LOG("post branch")

        UNUSED_PARAM(branch);
        _scope_stack.leave();
        return false;
    }
}

#undef KORE_DEBUG_TYPEINFERRER_LOG
#undef KORE_DEBUG_TYPEINFERRER_LOG_TYPE
