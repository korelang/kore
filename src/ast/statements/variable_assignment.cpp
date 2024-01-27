#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/expressions/expression.hpp"
#include "types/type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    VariableAssignment::VariableAssignment(
        bool is_mutable,
        const Type* type,
        Owned<Expression> lhs,
        Owned<Expression> rhs
    ) : Statement(lhs->location(), StatementType::VariableAssignment) {
        _mutable = is_mutable;
        _type = std::move(type);
        _lhs = std::move(lhs);
        _rhs = std::move(rhs);
        _location = SourceLocation(_lhs->location());
        _lhs->set_type(type);
    }

    VariableAssignment::~VariableAssignment() {}

    Expression* VariableAssignment::lhs() {
        return _lhs.get();
    }

    Expression* VariableAssignment::rhs() {
        return _rhs.get();
    }

    const Expression* VariableAssignment::lhs() const {
        return _lhs.get();
    }

    const Expression* VariableAssignment::rhs() const {
        return _rhs.get();
    }

    void VariableAssignment::set_type(const Type* type) {
        _lhs->set_type(type);
    }

    const Type* VariableAssignment::type() const {
        return _lhs->type();
    }

    const Type* VariableAssignment::declared_type() const {
        return _type;
    }

    bool VariableAssignment::is_mutable() const {
        return _mutable;
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(VariableAssignment)
}
