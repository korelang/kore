#include "ast/statements/statement.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Statement::Statement() : AstNode(SourceLocation::unknown) {}

    Statement::Statement(StatementType statement_type) : _statement_type(statement_type) {}

    Statement::Statement(SourceLocation location, StatementType statement_type)
        : AstNode(location), _statement_type(statement_type) {}

    Statement::~Statement() {}

    void Statement::add_statement(Owned<Statement> statement) {
        UNUSED_PARAM(statement);

        throw std::runtime_error("This statement type does not contain other statements");
    }

    StatementType Statement::statement_type() const {
        return _statement_type;
    }
}
