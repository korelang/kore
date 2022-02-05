#ifndef KORE_ARRAY_EXPRESSION_HPP
#define KORE_ARRAY_EXPRESSION_HPP

#include <vector>

#include "ast/expressions/expression.hpp"

namespace kore {
    class ArrayType;

    class ArrayExpression : public Expression {
        public:
            ArrayExpression();
            ArrayExpression(const Location& location);
            virtual ~ArrayExpression();

            void set_start_location(const Location& location);
            void set_end_location(const Location& location);
            void add_element(Expression* expr);
            bool uses_constants_only() const;
            const Type* type() const override;

            void write(AstWriter* const writer) override;

        private:
            Location _start, _end;
            std::vector<pointer> _elements;
            ArrayType* _type;
    };
}

#endif // KORE_ARRAY_EXPRESSION_HPP
