#ifndef KORE_VARIABLE_ASSIGNMENT_HPP
#define KORE_VARIABLE_ASSIGNMENT_HPP

#include <vector>

#include "ast/statements/statement.hpp"

namespace kore {
    class Expression;
    class Type;

    /// A multi-variable assignment e.g.
    ///
    /// * a.lol, b[0], var c = d()
    /// * var a i32, b, c str = 1, false, "hello"
    class VariableAssignment : public Statement {
        public:
            VariableAssignment(
                std::vector<Owned<Expression>> lhs_exprs,
                std::vector<Owned<Expression>> rhs_exprs
            );

            virtual ~VariableAssignment();

            int lhs_count() const;
            int rhs_count() const;
            Expression* lhs(int idx);
            Expression* rhs(int idx);
            const Expression* lhs(int idx) const;
            const Expression* rhs(int idx) const;
            const Type* rhs_type(int idx) const;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            std::vector<Owned<Expression>> _lhs_exprs;
            std::vector<Owned<Expression>> _rhs_exprs;
    };
}

#endif // KORE_VARIABLE_ASSIGNMENT_HPP
