#ifndef KORE_PARAMETER_HPP
#define KORE_PARAMETER_HPP

#include "ast/expressions/identifier.hpp"

namespace kore {
    /// Ast node for function parameters
    // TODO: Make it independent of identifiers as its type is part of the function type
    class Parameter : public Identifier {
        public:
            Parameter(const Token& token);
            Parameter(const Token& token, const Type* type);
            virtual ~Parameter();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION
    };
}

#endif // KORE_PARAMETER_HPP
