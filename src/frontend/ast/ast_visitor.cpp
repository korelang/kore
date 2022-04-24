#include "ast_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstVisitor::AstVisitor() {}

    AstVisitor::~AstVisitor() {}

    void AstVisitor::visit(ArrayExpression& expr) {
        UNUSED_PARAM(expr);
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

    void AstVisitor::visit_only(ArrayExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(ArrayFillExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(ArrayRangeExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(BinaryExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(BoolExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(CharExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(FloatExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(Identifier& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(Parameter& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(IntegerExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(StringExpression& expr) {
        UNUSED_PARAM(expr);
    }

    void AstVisitor::visit_only(UnaryExpression& expr) {
        UNUSED_PARAM(expr);
    }


    bool AstVisitor::precondition(ArrayExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(ArrayFillExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(ArrayRangeExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(BinaryExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(BoolExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(CharExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(FloatExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(Identifier& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(IntegerExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(StringExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
    }

    bool AstVisitor::precondition(UnaryExpression& expr) {
        UNUSED_PARAM(expr);
        return false;
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

    void AstVisitor::visit_only(Branch& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(Function& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(class Call& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(IfStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(ImportStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(ModuleStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(Return& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(VariableAssignment& statement) {
        UNUSED_PARAM(statement);
    }

    void AstVisitor::visit_only(VariableDeclaration& statement) {
        UNUSED_PARAM(statement);
    }


    bool AstVisitor::precondition(Branch& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(Function& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(class Call& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(IfStatement& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(ImportStatement& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(ModuleStatement& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(Return& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(VariableAssignment& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::precondition(VariableDeclaration& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(Branch& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(Function& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(class Call& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(IfStatement& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(ImportStatement& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(ModuleStatement& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(Return& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(VariableAssignment& statement) {
        UNUSED_PARAM(statement);
        return false;
    }

    bool AstVisitor::postcondition(VariableDeclaration& statement) {
        UNUSED_PARAM(statement);
        return false;
    }
}
