#ifndef KORE_PARAMETER_HPP
#define KORE_PARAMETER_HPP

#include "ast/expressions/identifier.hpp"

namespace kore {
    /// Ast node for function parameters
    class Parameter : public Identifier {
        public:
            Parameter(const Token& token);
            Parameter(const Token& token, Type* type);
            virtual ~Parameter();

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            std::string _name;
    };
}

#endif // KORE_PARAMETER_HPP
