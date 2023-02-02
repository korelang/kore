#include "ast/ast_visitor.hpp"
#include "ast/statements/branch.hpp"
#include "if_statement.hpp"

namespace kore {
    IfStatement::IfStatement() : _has_else_branch(false) {
    }

    IfStatement::~IfStatement() {
    }

    void IfStatement::add_branch(Owned<Expression> condition) {
        _branches.emplace_back(
            new Branch(std::move(condition), std::move(_statement_accumulator))
        );
        _statement_accumulator.clear();
    }

    void IfStatement::add_else_branch() {
        _has_else_branch = true;
        _branches.emplace_back(new Branch(std::move(_statement_accumulator)));
        _statement_accumulator.clear();
    }

    void IfStatement::add_statement(Owned<Statement> statement) {
        _statement_accumulator.emplace_back(std::move(statement));
    }

    bool IfStatement::has_else_branch() const {
        return _has_else_branch;
    }

    int IfStatement::branch_count() const {
        return _branches.size();
    }

    branch_iterator IfStatement::begin() const {
        return _branches.cbegin();
    }

    branch_iterator IfStatement::end() const {
        return _branches.cend();
    }

    branch_ptr& IfStatement::operator[](int index) {
        return _branches[index];
    }

    branch_ptr& IfStatement::last_branch() {
        return _branches[_branches.size()-1];
    }

    void IfStatement::accept(AstVisitor& visitor) {
        for (auto& branch : _branches) {
            branch->accept(visitor);
        }

        visitor.visit(*this);
    }

    void IfStatement::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
