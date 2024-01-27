#include "ast_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstVisitor::AstVisitor() {}

    AstVisitor::~AstVisitor() {}

    void AstVisitor::visit(Expression& expr, ValueContext context) {
        UNUSED_PARAM(expr);
        UNUSED_PARAM(context);
    }

    void AstVisitor::visit(ArrayExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(IndexExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(IndexExpression& expr, ValueContext context) {
        UNUSED_PARAM(expr);
        UNUSED_PARAM(context);
    }

    void AstVisitor::visit(ArrayFillExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(ArrayRangeExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(BinaryExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(BoolExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(CharExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(FloatExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(Identifier& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(Identifier& expr, ValueContext context) {
        UNUSED_PARAM(expr);
        UNUSED_PARAM(context);
    }

    void AstVisitor::visit(Parameter& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(IntegerExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(StringExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(UnaryExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit(Branch& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(Function& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(class Call& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(IfStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(ImportStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(ModuleStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(Return& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(VariableAssignment& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(VariableDeclaration& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit(ExpressionStatement& exprstmt) {
        UNUSED_PARAM(exprstmt);
    }
}
