#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/statements/branch.hpp"
#include "if_statement.hpp"

namespace kore {
    IfStatement::IfStatement() : _has_else_branch(false) {
    }

    IfStatement::~IfStatement() {
    }

    void IfStatement::add_branch(Expression* condition) {
        _branches.emplace_back(new Branch(condition, std::move(_statement_accumulator)));
        _statement_accumulator.clear();
    }

    void IfStatement::add_else_branch() {
        _has_else_branch = true;
        _branches.emplace_back(new Branch(std::move(_statement_accumulator)));
        _statement_accumulator.clear();
    }

    void IfStatement::add_statement(Statement* statement) {
        _statement_accumulator.emplace_back(statement);
    }

    bool IfStatement::has_else_branch() const {
        return _has_else_branch;
    }

    branch_iterator IfStatement::begin() const {
        return _branches.cbegin();
    }

    branch_iterator IfStatement::end() const {
        return _branches.cend();
    }

    void IfStatement::write(AstWriter* const writer) {
        writer->write("if ");
        _branches[0]->write(writer);

        for (std::vector<Branch>::size_type i = 1; i < _branches.size() - 1; ++i) {
            writer->write("else if ");
            _branches[i]->write(writer);
        }

        if (has_else_branch()) {
            writer->write("else ");
            _branches.back()->write(writer);
        }
    }

    void IfStatement::accept(AstVisitor* visitor) {
        for (auto& branch : _branches) {
            branch->accept(visitor);
        }

        visitor->visit(this);
    }
}
