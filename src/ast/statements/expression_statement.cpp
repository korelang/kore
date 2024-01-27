#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/expression_statement.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    ExpressionStatement::ExpressionStatement(Owned<Expression> expression)
        : _expression(std::move(expression)) {}

    ExpressionStatement::~ExpressionStatement() {}

    Expression* ExpressionStatement::expression() {
        return _expression.get();
    }

    void ExpressionStatement::accept(AstVisitor& visitor) {
        if (_expression) {
            _expression->accept(visitor);
        }
    }

    void ExpressionStatement::accept(AstVisitor& visitor, ValueContext context) {
        UNUSED_PARAM(context);
        accept(visitor);
    }
}
