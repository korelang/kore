#ifndef KORE_VARIABLE_DECLARATION_HPP
#define KORE_VARIABLE_DECLARATION_HPP

#include "ast/statements/statement.hpp"
#include "token.hpp"

namespace kore {
    class VariableDeclaration : public Statement {
        public:
            VariableDeclaration(const Token& identifier, const Token& type);
            virtual ~VariableDeclaration();

            std::string identifier() const;
            std::string type() const;

            void write(AstWriter* const writer) override;

        private:
            std::string _identifier;
            std::string _type;
    };
}

#endif // KORE_VARIABLE_DECLARATION_HPP
