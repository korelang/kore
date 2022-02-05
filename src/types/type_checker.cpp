#include "type_checker.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "utils/unused_parameter.hpp"

#include <sstream>

TypeError::TypeError(const std::string& message, const Location& location)
    : message(message),
      location(location) {}

TypeChecker::TypeChecker() {}

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

void TypeChecker::visit(VariableAssignment* statement) {
    auto explicit_type = statement->type();
    auto expr_type = statement->expression()->type();

    if (explicit_type->unify(expr_type)->is_unknown()) {
        std::ostringstream oss;

        oss << "cannot assign expression of type "
            << statement->expression()->type()->name()
            << " to variable of type "
            << statement->type()->name()
            << " without conversion";

        push_error(oss.str(), statement->location());
    }
}

void TypeChecker::visit(BinaryExpression* expr) {
    auto left = expr->left();
    auto right = expr->right();
    auto op = expr->op();

    if (op == "+" || op == "-" || op == "*" || op == "/") {
        auto left_type = left->type();
        auto right_type = right->type();

        if (left_type->is_numeric() && right_type->is_numeric()) {
            auto result_type = left_type->unify(right_type);

            if (result_type->is_unknown()) {
                std::ostringstream oss;

                oss << "cannot use binary operation '" << op << "'"
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
