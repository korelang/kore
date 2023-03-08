#include <exception>

#include "ast/expressions/expression.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/statements/function.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/statements/module_statement.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/statements/variable_declaration.hpp"
#include "ast/statements/expression_statement.hpp"
#include "types/type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Statement::Statement() : AstNode(SourceLocation::unknown) {}

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
