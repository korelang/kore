#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_declaration.hpp"

namespace kore {
    VariableDeclaration::VariableDeclaration(const Token& identifier, const Token& type)
        : Statement(),
        _identifier(identifier.value()),
        _type(type.value())
    {
        _location = SourceLocation(
            identifier.location().lnum(),
            identifier.location().start(),
            type.location().end()
        );
    }

    VariableDeclaration::~VariableDeclaration() {}

    std::string VariableDeclaration::identifier() const {
        return _identifier;
    }

    std::string VariableDeclaration::type() const {
        return _type;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(VariableDeclaration)
}
