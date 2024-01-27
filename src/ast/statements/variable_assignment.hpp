#ifndef KORE_VARIABLE_ASSIGNMENT_HPP
#define KORE_VARIABLE_ASSIGNMENT_HPP

#include "ast/expressions/identifier.hpp"
#include "ast/statements/statement.hpp"
#include "ast/scanner/token.hpp"

namespace kore {
    class Expression;
    class Type;

    class VariableAssignment : public Statement {
        public:
            VariableAssignment(
                bool is_mutable,
                const Type* type,
                Owned<Expression> lhs,
                Owned<Expression> rhs
            );
            virtual ~VariableAssignment();

            Expression* lhs();
            Expression* rhs();
            const Expression* lhs() const;
            const Expression* rhs() const;
            const Type* type() const;
            const Type* declared_type() const;
            void set_type(const Type* type);
            bool is_mutable() const;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            bool _mutable;
            const Type* _type;
            Owned<Expression> _lhs;
            Owned<Expression> _rhs;
    };
}

#endif // KORE_VARIABLE_ASSIGNMENT_HPP
