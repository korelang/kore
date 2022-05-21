#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/parameter.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    Parameter::Parameter(const Token& token) : Identifier(token, nullptr) {
        set_type(Type::unknown());
    }

    Parameter::Parameter(const Token& token, Type* type)
        : Identifier(token, type) {
    }

    Parameter::~Parameter() {}

    void Parameter::accept(AstVisitor& visitor) {
        accept_visit_only(visitor);
    }

    void Parameter::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
