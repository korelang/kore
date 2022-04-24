#include "optimiser.hpp"

namespace kore {
    void Optimiser::optimise(const Ast& ast) {
        for (auto const& statement : ast) {
            statement->accept(*this);
        }
    }

    void Optimiser::fold_numeric_literals(
        const std::string& op,
        const Expression* left,
        const Expression* right
    ) {
        optimise(left);
        optimise(right);

        i32 result = 0;

        if (op == "+") {
            result = left->value() + right->value();
        }

        // TODO: How to update the location of the new token? Is this easier to do
        // in the parser? Merge the location of the two tokens since this
        // optimisation should be transparent to the user i.e. the position should
        // remain the same
        return new IntegerExpression(
            result,
            Location(left->location(), right->location())
        );
    }

    void Optimiser::visit(BinaryExpression* expr) {
        bool left_is_literal = expr->left()->is_literal();
        bool right_is_literal = expr->right()->is_literal();

        if (left_is_literal && right_is_literal) {
            // Assume literals are numeric for now
            return fold_numeric_literals(expr->left(), expr->right());
        } else {
            return expr;
        }
    }

    void Optimiser::visit(VariableAssignment* statement) {
        statement->set_expr(...);
    }
}
