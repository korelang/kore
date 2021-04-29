#ifndef KORE_VARIABLE_ASSIGNMENT_HPP
#define KORE_VARIABLE_ASSIGNMENT_HPP

#include "ast/statements/statement.hpp"
#include "token.hpp"

class Expression;
class Type;

class VariableAssignment : public Statement {
    public:
        VariableAssignment(const Token& identifier, Type* type, Expression* expr);
        virtual ~VariableAssignment();

        std::string identifier() const;

        void write(AstWriter* const writer) override;

    private:
        std::string _identifier;
        std::unique_ptr<Type> _type;
        std::unique_ptr<Expression> _expr;
};

#endif // KORE_VARIABLE_ASSIGNMENT_HPP
