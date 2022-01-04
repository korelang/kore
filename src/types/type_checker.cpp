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

    if (explicit_type->unify(expr_type)->category() == TypeCategory::Unknown) {
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

            if (result_type->category() == TypeCategory::Unknown) {
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

bool TypeChecker::check_statement(Statement* const statement) {
   switch (statement->statement_type()) {
       // These statements are not checked but might be later
       case StatementType::ModuleDecl:
       case StatementType::ImportDecl:
           return true;

       case StatementType::VariableDecl:
           return check_variable_declaration(static_cast<VariableDeclaration*>(statement));

       case StatementType::VariableAssignment:
           return check_variable_assignment(static_cast<VariableAssignment*>(statement));
   }
}

bool TypeChecker::check_variable_declaration(VariableDeclaration* const decl) {
    UNUSED_PARAM(decl);

    return true;
}

bool TypeChecker::check_variable_assignment(VariableAssignment* const assign) {
    // If the right-hand side has a type error, don't bother checking
    // the assignment's left-hand side
    if (!check_expression(assign->expression())) {
        return false;
    }

    if (assign->type() != assign->expression()->type()) {
        push_error(
            "Cannot assign expression of type '" +
            assign->type()->name() +
            "' to variable of type '" +
            assign->expression()->type()->name(),
            assign->location()
        );

        return false;
    }

    return true;
}

bool TypeChecker::check_expression(const Expression* const expression) {
    switch (expression->expr_type()) {
        case ExpressionType::Binary:
            return check_binary_expression(static_cast<const BinaryExpression*>(expression));

        case ExpressionType::Literal:
            // A literal is already correctly typed
            return true;

        case ExpressionType::Array:
        case ExpressionType::Identifier:
        case ExpressionType::Unary:
            return false;

        case ExpressionType::Error:
            return false;
    }

    return false;
}

bool TypeChecker::check_binary_expression(const BinaryExpression* const binexpr) {
    bool lhs_check = check_expression(binexpr->left());
    bool rhs_check = check_expression(binexpr->right());

    if (!lhs_check || !rhs_check) {
        // Don't bother checking the binary expression if its operands have
        // type errors
        return false;
    }

    auto lhs_type = binexpr->left()->type();
    auto rhs_type = binexpr->right()->type();

    if (!lhs_type->is_numeric() || !rhs_type->is_numeric()) {
        push_error(
            "binary expression must have numeric operands",
            binexpr->location()
        );

        return false;
    }

    return true;
}
