#ifndef KORE_CALL_HPP
#define KORE_CALL_HPP

#include <vector>

#include "ast/expressions/expression.hpp"

namespace kore {
    class Call : public Expression {
        public:
            Call(Owned<Expression> identifier, std::vector<Owned<Expression>> parameters);
            virtual ~Call();

            std::string name() const;
            Expression* arg(int index);
            /* std::vector<Expression*> parameters(); */

            int arg_count() const;
            std::string expected_func_type_name() const;
            const Type* type() const override;

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            Expression::pointer _identifier;
            std::vector<Expression::pointer> _parameters;
    };
}

#endif // KORE_CALL_HPP
