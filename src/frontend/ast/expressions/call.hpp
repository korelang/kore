#ifndef KORE_CALL_HPP
#define KORE_CALL_HPP

#include <vector>

#include "ast/expressions/expression.hpp"

namespace kore {
    class Call : public Expression {
        public:
            Call(Identifier* identifier, std::vector<Expression*>& parmeters);
            virtual ~Call();

            std::string name() const;
            Expression* arg(int index);
            /* std::vector<Expression*> parameters(); */

            int arg_count() const;
            std::string expected_func_type_name() const;
            const Type* type() const override;

            void write(AstWriter* const writer) override;
            void accept(AstVisitor& visitor) override;

        private:
            std::string _name;
            std::vector<Expression::pointer> _parameters;
    };
}

#endif // KORE_CALL_HPP
