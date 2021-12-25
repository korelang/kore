#include "ast/ast_writer.hpp"
#include "ast/statements/branch.hpp"
#include "if_statement.hpp"

IfStatement::IfStatement() : _has_else_branch(false) {
}

IfStatement::~IfStatement() {
}

void IfStatement::add_branch(Expression* condition) {
    branches.emplace_back(new Branch(condition, std::move(statement_accumulator)));
    statement_accumulator.clear();
}

void IfStatement::add_else_branch() {
    _has_else_branch = true;
    branches.emplace_back(new Branch(std::move(statement_accumulator)));
    statement_accumulator.clear();
}

void IfStatement::add_statement(Statement* statement) {
    statement_accumulator.emplace_back(statement);
}

bool IfStatement::has_else_branch() const {
    return _has_else_branch;
}

void IfStatement::write(AstWriter* const writer) {
    writer->write("if ");
    branches[0]->write(writer);

    for (std::vector<Branch>::size_type i = 1; i < branches.size() - 1; ++i) {
        writer->write("else if ");
        branches[i]->write(writer);
    }

    if (has_else_branch()) {
        writer->write("else ");
        branches.back()->write(writer);
    }
}
