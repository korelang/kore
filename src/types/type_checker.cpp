#include "type_checker.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "utils/unused_parameter.hpp"

#include <sstream>

namespace kore {
    TypeError::TypeError(const std::string& message, const Location& location)
        : message(message),
        location(location) {}

    TypeChecker::TypeChecker(SymbolTable& symbol_table)
        : _symbol_table(symbol_table) {}

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
        if (!_symbol_table.find_identifier(expr->name())) {
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

        _symbol_table.insert_identifier(statement->identifier());
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
}
