#include "ast/statements/statements.hpp"
#include "ast_statement_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstStatementVisitor::AstStatementVisitor() {}

    AstStatementVisitor::~AstStatementVisitor() {}

    void AstStatementVisitor::visit(Branch& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(Function& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(class Call& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(IfStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(ImportStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(ModuleStatement& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(Return& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(VariableAssignment& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(VariableDeclaration& statement) {
        UNUSED_PARAM(statement);
    }

    void AstStatementVisitor::visit(ExpressionStatement& exprstmt) {
        UNUSED_PARAM(exprstmt);
    }
}
