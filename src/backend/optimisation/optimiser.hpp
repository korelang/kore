#ifndef KORE_OPTIMISER_HPP
#define KORE_OPTIMISER_HPP

#include "ast/ast_visitor.hpp"

namespace kore {
    class Optimiser : public AstVisitor {
        void optimise(const Ast& ast);

        void optimise_constant_folding();

        Expression* fold_numeric_literals(
            const std::string& op,
            const Expression* left,
            const Expression* right
        );

        /* void visit(ArrayExpression* expr) override; */
        /* void visit(ArrayFillExpression* expr) override; */
        /* void visit(ArrayRangeExpression* expr) override; */
        void visit(BinaryExpression* expr) override;
        /* void visit(BoolExpression* expr) override; */
        /* void visit(CharExpression* expr) override; */
        /* void visit(FloatExpression* expr) override; */
        /* void visit(Identifier* expr) override; */
        /* void visit(IntegerExpression* expr) override; */
        /* void visit(StringExpression* expr) override; */
        /* void visit(UnaryExpression* expr) override; */

        void visit(VariableAssignment* statement) override;
    }
}

#endif // KORE_OPTIMISER_HPP
