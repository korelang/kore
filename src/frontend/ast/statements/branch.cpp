#include "ast/ast_writer.hpp"
#include "branch.hpp"
#include "ast/expressions/expression.hpp"

namespace kore {
    Branch::Branch(Expression* condition, StatementList statements)
        : _condition(condition),
        _statements(std::move(statements)) {
    }

    Branch::Branch(StatementList statements)
        : _statements(std::move(statements)) {
    }

    Branch::~Branch() {
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
}
