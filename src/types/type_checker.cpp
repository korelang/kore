#include "type_checker.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/branch.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "errors/errors.hpp"
#include "types/function_type.hpp"
#include "utils/unused_parameter.hpp"

#include <sstream>

#if defined(KORE_DEBUG_TYPECHECKER) || defined(KORE_DEBUG)
    #include <iostream>

    #define KORE_DEBUG_TYPECHECKER_LOG(prefix, typename1, typename2) {\
        std::cerr << prefix;\
        \
        if (!typename1.empty()) {\
            std::cerr << ": " << typename1;\
        }\
        \
        if (!typename2.empty()) {\
            std::cerr << ", " << typename2;\
        }\
        \
        std::cerr << std::endl;\
    }
#else
    #define KORE_DEBUG_TYPECHECKER_LOG(prefix, type1, type2)
#endif

namespace kore {
    TypeChecker::TypeChecker(ScopeStack& scope_stack)
        : _scope_stack(scope_stack) {}

    TypeChecker::~TypeChecker() {}

    int TypeChecker::check(const Ast& ast) {
        _errors.clear();

        for (auto const& statement : ast) {
            statement->accept(this);
        }

        return _errors.size();
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

    void TypeChecker::visit(Identifier* expr) {
        auto entry = _scope_stack.find(expr->name());

        if (entry) {
            KORE_DEBUG_TYPECHECKER_LOG(
                "identifier",
                entry->identifier->type()->name(),
                std::string()
            )

            expr->set_type(entry->identifier->type());
        } else {
            push_error(errors::typing::undefined_variable(expr));
        }
    }

    void TypeChecker::visit(VariableAssignment* statement) {
        auto declared_type = statement->declared_type();
        auto expr_type = statement->expression()->type();
        KORE_DEBUG_TYPECHECKER_LOG(
            "assignment",
            declared_type->name(),
            expr_type->name()
        )

        // If the variable was not given an explicit type, rely on inferred
        // type instead
        if (!declared_type->is_unknown()) {
            if (declared_type->unify(expr_type)->is_unknown()) {
                push_error(errors::typing::cannot_assign(expr_type, declared_type, statement->location()));
            }
        }

        auto identifier = statement->identifier();
        auto entry = _scope_stack.find_inner(identifier->name());

        if (!entry) {
            _scope_stack.insert(identifier);

            // This variable did not already exist in the inner scope, check
            // that it does not shadow a variable in an outer scope
            if (shadows_outer_scope(identifier)) {
                push_error(errors::typing::variables_shadows(identifier, statement->location()));
            }
        } else {
            // TODO
            // Variable already exists in this scope, check that we are not
            // redeclaring a constant variable
        }
    }

    void TypeChecker::visit(class Call* statement) {
        // Find an scoped entry in the current or enclosing scopes for a
        // function definition
        auto entry = _scope_stack.find(statement->name());

        if (!entry) {
            push_error(errors::typing::unknown_call(statement));
            return;
        }

        auto type = entry->identifier->type();

        if (type->category() != TypeCategory::Function) {
            push_error(errors::typing::not_a_function(statement, type));
            return;
        }

        auto func_type = static_cast<const FunctionType*>(type);

        if (statement->arg_count() != func_type->arity()) {
            push_error(errors::typing::incorrect_arg_count(
                statement,
                func_type
            ));

            return;
        }

        KORE_DEBUG_TYPECHECKER_LOG("call", func_type->name(), std::string())

        // TODO: Move into Call class
        for (int i = 0; i < statement->arg_count(); ++i) {
            auto arg = statement->arg(i);
            auto arg_type = arg->type();
            auto param = func_type->parameter(i);
            auto param_type = param->type();
            auto unified_type = arg_type->unify(param_type);

            if (unified_type->is_unknown()) {
                push_error(errors::typing::incorrect_parameter_type(
                    arg,
                    arg_type,
                    param_type,
                    statement,
                    i
                ));
            }
        }

        statement->set_type(func_type->return_type());
    }

    void TypeChecker::visit(BinaryExpression* expr) {
        auto left = expr->left();
        auto right = expr->right();
        auto op = expr->op();

        KORE_DEBUG_TYPECHECKER_LOG(
            "binop",
            left->type()->name(),
            right->type()->name()
        )

        if (op == BinOp::Plus || op == BinOp::Minus || op == BinOp::Mult || op == BinOp::Div) {
            auto left_type = left->type();
            auto right_type = right->type();

            if (left_type->is_numeric() && right_type->is_numeric()) {
                auto result_type = left_type->unify(right_type);

                if (result_type->is_unknown()) {
                    push_error(errors::typing::incompatible_binop(left_type, right_type, op, expr->location()));
                } else {
                    expr->set_type(result_type);
                }
            } else {
                push_error(errors::typing::binop_numeric_operands(left_type, right_type, op, expr->location()));
            }
        }
    }

    bool TypeChecker::precondition(Branch* branch) {
        KORE_DEBUG_TYPECHECKER_LOG("pre branch", std::string(), std::string())

        UNUSED_PARAM(branch);
        _scope_stack.enter();
        return false;
    }

    bool TypeChecker::postcondition(Branch* branch) {
        KORE_DEBUG_TYPECHECKER_LOG("post branch", std::string(), std::string())

        UNUSED_PARAM(branch);
        _scope_stack.leave();
        return false;
    }

    bool TypeChecker::precondition(Function* statement) {
        KORE_DEBUG_TYPECHECKER_LOG("pre function", std::string(), std::string())
        UNUSED_PARAM(statement);

        // Enter a new function scope and add all function
        // arguments to that scope
        _scope_stack.enter_function_scope();

        for (int i = 0; i < statement->arity(); ++i) {
            auto parameter = statement->parameter(i);
            _scope_stack.insert(parameter);
        }

        return false;
    }

    bool TypeChecker::postcondition(Function* statement) {
        KORE_DEBUG_TYPECHECKER_LOG(
            "post function",
            std::string(),
            std::string()
        )

        UNUSED_PARAM(statement);
        _scope_stack.leave();
        return false;
    }

    bool TypeChecker::shadows_outer_scope(const Identifier* identifier) {
        return _scope_stack.find_enclosing(identifier->name()) != nullptr;
    }
}

#undef KORE_DEBUG_TYPECHECKER_LOG
