#include "ast/expressions/expressions.hpp"
#include "ast_expr_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstExprVisitor::AstExprVisitor() {}

    AstExprVisitor::~AstExprVisitor() {}

    void AstExprVisitor::visit(Expression& expr, ValueContext context) {
        UNUSED_PARAM(expr);
        UNUSED_PARAM(context);
    }

    void AstExprVisitor::visit(ArrayExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(IndexExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(IndexExpression& expr, ValueContext context) {
        UNUSED_PARAM(expr);
        UNUSED_PARAM(context);
    }

    void AstExprVisitor::visit(ArrayFillExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(ArrayRangeExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(BinaryExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(BoolExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(CharExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(FloatExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(FieldAccessExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(Identifier& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(Identifier& expr, ValueContext context) {
        UNUSED_PARAM(expr);
        UNUSED_PARAM(context);
    }

    void AstExprVisitor::visit(Parameter& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(IntegerExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(StringExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstExprVisitor::visit(UnaryExpression& expr) {
        UNUSED_PARAM(expr);
    }
}
