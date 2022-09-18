#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/expressions/expression.hpp"
#include "types/type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    VariableAssignment::VariableAssignment(
        bool is_mutable,
        const Token& identifier,
        Type* type,
        Expression* expr
    )
        : Statement(identifier.location(), StatementType::VariableAssignment),
        _identifier(identifier, is_mutable),
        _type(std::move(type)),
        _expr(std::move(expr))
    {
        _location = Location(
            identifier.location().lnum(),
            identifier.location().start(),
            expr->location().end()
        );

        _identifier.set_type(type);
    }

    VariableAssignment::~VariableAssignment() {}

    const Identifier* VariableAssignment::identifier() const {
        return &_identifier;
    }

    Expression* VariableAssignment::expression() const {
        return _expr.get();
    }

    void VariableAssignment::set_type(const Type* type) {
        _identifier.set_type(type);
    }

    const Type* VariableAssignment::type() const {
        return _identifier.type();
    }

    const Type* VariableAssignment::declared_type() const {
        return _type.get();
    }

    void VariableAssignment::accept(AstVisitor& visitor) {
        _expr->accept(visitor);
        /* _identifier.accept(visitor); */

        visitor.visit(*this);
    }

    void VariableAssignment::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
