#include "type_checker.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/branch.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "utils/unused_parameter.hpp"

#include <sstream>

namespace kore {
    TypeError::TypeError(const std::string& message, const Location& location)
        : message(message),
        location(location) {}

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

    std::vector<TypeError> TypeChecker::errors() {
        return _errors;
    }

    void TypeChecker::push_error(
        const std::string& message,
        const Location& location
    ) {
        _errors.emplace_back(message, location);
    }

    /* void TypeChecker::visit(IfStatement* statement) { */
    /*     for (auto branch : statement->branches()) { */
    /*         branch->accept(this); */
    /*     } */
    /* } */

    void TypeChecker::visit(Identifier* expr) {
        auto entry = _scope_stack.find(expr->name());

        if (!entry) {
            push_error("use of undefined variable " + expr->name(), expr->location());
        }
    }

    void TypeChecker::visit(VariableAssignment* statement) {
        auto declared_type = statement->declared_type();
        auto expr_type = statement->expression()->type();

        // If the variable was not given an explicit type, rely on inferred
        // type instead
        if (!declared_type->is_unknown()) {
            if (declared_type->unify(expr_type)->is_unknown()) {
                std::ostringstream oss;

                oss << "cannot assign expression of type "
                    << expr_type->name()
                    << " to variable of type "
                    << declared_type->name()
                    << " without conversion";

                push_error(oss.str(), statement->location());
            }
        }

        auto identifier = statement->identifier();
        auto entry = _scope_stack.find_inner(identifier->name());

        if (!entry) {
            _scope_stack.insert(identifier);

            // This variable did not already exist in the inner scope, check
            // that it does not shadow a variable in an outer scope
            if (shadows_outer_scope(identifier)) {
                auto message =
                    "variable " +
                    identifier->name() +
                    " shadows variable in outer scope";

                push_error(message, statement->location());
            }
        } else {
            // TODO
            // Variable already exists in this scope, check that we are not
            // redeclaring a constant variable
        }
    }

    void TypeChecker::visit(BinaryExpression* expr) {
        auto left = expr->left();
        auto right = expr->right();
        auto op = expr->op();

        if (op == BinOp::Plus || op == BinOp::Minus || op == BinOp::Mult || op == BinOp::Div) {
            auto left_type = left->type();
            auto right_type = right->type();

            if (left_type->is_numeric() && right_type->is_numeric()) {
                auto result_type = left_type->unify(right_type);

                if (result_type->is_unknown()) {
                    std::ostringstream oss;

                    oss << "cannot use binary operation '" << binop_to_string(op) << "'"
                        << " with numeric types "
                        << left_type->name()
                        << " and "
                        << right_type->name()
                        << " without conversion";

                    push_error(oss.str(), expr->location());
                } else {
                    expr->set_type(result_type);
                }
            } else {
                std::ostringstream oss;

                oss << "binary expression must have numeric operands but got "
                    << left_type->name()
                    << " and "
                    << right_type->name();

                push_error(oss.str(), expr->location());
            }
        }
    }

    bool TypeChecker::precondition(Branch* branch) {
        UNUSED_PARAM(branch);
        _scope_stack.enter(false);
        return false;
    }

    bool TypeChecker::postcondition(Branch* branch) {
        UNUSED_PARAM(branch);
        _scope_stack.leave();
        return false;
    }

    bool TypeChecker::shadows_outer_scope(const Identifier* identifier) {
        return _scope_stack.find_enclosing(identifier->name()) != nullptr;
    }
}
