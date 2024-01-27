#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/parameter.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    Parameter::Parameter(const Token& token) : Identifier(token, nullptr) {
        set_type(Type::unknown());
    }

    Parameter::Parameter(const Token& token, const Type* type)
        : Identifier(token, type) {
    }

    Parameter::~Parameter() {}

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(Parameter)
}
