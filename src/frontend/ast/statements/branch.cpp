#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "branch.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    Branch::Branch(Expression* condition, StatementList statements)
        : _condition(condition),
        _statements(std::move(statements)) {
    }

    Branch::Branch(StatementList statements) : Branch(nullptr, std::move(statements)) {
    }

    Branch::~Branch() {
    }

    Expression* Branch::condition() {
        return _condition;
    }

    body_iterator Branch::begin() const {
        return _statements.begin();
    }

    body_iterator Branch::end() const {
        return _statements.end();
    }

    void Branch::write(AstWriter* const writer) {
        // Only "if" and "else if" branches have conditions
        if (_condition) {
            _condition->write(writer);
        }

        writer->write("{\n");
        
        for (auto&& statement : _statements) {
            statement->write(writer);
        }

        writer->write("}\n");
    }

    void Branch::accept(AstVisitor* visitor) {
        if (visitor->precondition(this)) {
            return;
        }

        if (_condition) {
            _condition->accept(visitor);
        }

        for (auto& statement : _statements) {
            statement->accept(visitor);
        }

        if (visitor->postcondition(this)) {
            return;
        }
    }
}
