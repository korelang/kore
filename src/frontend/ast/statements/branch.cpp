#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "branch.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    Branch::Branch(Owned<Expression> condition, StatementList statements)
        : _condition(std::move(condition)),
        _statements(std::move(statements)) {
    }

    Branch::Branch(StatementList statements) : Branch(nullptr, std::move(statements)) {
    }

    Branch::~Branch() {
    }

    Expression* Branch::condition() {
        return _condition.get();
    }

    body_iterator Branch::begin() const {
        return _statements.begin();
    }

    body_iterator Branch::end() const {
        return _statements.end();
    }

    void Branch::accept(AstVisitor& visitor) {
        if (_condition) {
            _condition->accept(visitor);
        }

        for (auto& statement : _statements) {
            statement->accept(visitor);
        }

        visitor.visit(*this);
    }

    void Branch::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
