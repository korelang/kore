#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/expression_statement.hpp"

namespace kore {
    ExpressionStatement::ExpressionStatement(Expression* expression)
        : _expression(expression) {}

    ExpressionStatement::~ExpressionStatement() {}

    Expression* ExpressionStatement::expression() {
        return _expression.get();
    }

    void ExpressionStatement::accept(AstVisitor& visitor) {
        if (_expression) {
            _expression->accept(visitor);
        }
    }

    void ExpressionStatement::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
