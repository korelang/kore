#ifndef KORE_VARIABLE_ASSIGNMENT_HPP
#define KORE_VARIABLE_ASSIGNMENT_HPP

#include "ast/expressions/identifier.hpp"
#include "ast/statements/statement.hpp"
#include "token.hpp"

namespace kore {
    class Expression;
    class Type;

    class VariableAssignment : public Statement {
        public:
            VariableAssignment(const Token& identifier, Type* type, Expression* expr);
            virtual ~VariableAssignment();

            const Identifier* identifier() const;
            Expression* expression() const;
            const Type* type() const;
            const Type* declared_type() const;
            void set_type(const Type* type);

            void write(AstWriter* const writer) override;
            void accept(AstVisitor* visitor) override;

        private:
            // TODO: Should this be a variable instead? It is more specific in
            // this context
            Identifier _identifier;
            std::unique_ptr<const Type> _type;
            std::unique_ptr<Expression> _expr;
    };
}

#endif // KORE_VARIABLE_ASSIGNMENT_HPP
