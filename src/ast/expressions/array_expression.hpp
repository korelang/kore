#ifndef KORE_ARRAY_EXPRESSION_HPP
#define KORE_ARRAY_EXPRESSION_HPP

#include <vector>

#include "ast/expressions/expression.hpp"

namespace kore {
    class ArrayType;

    class ArrayExpression : public Expression {
        public:
            ArrayExpression();
            ArrayExpression(const SourceLocation& location);
            virtual ~ArrayExpression();

            void set_start_location(const SourceLocation& location);
            void set_end_location(const SourceLocation& location);
            void add_element(Owned<Expression> expr);
            bool uses_constants_only() const;
            int size() const;
            Expression* operator[](int index);
            const Type* type() const override;
            const ArrayType* array_type() const;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            std::vector<Owned<Expression>> _elements;
    };
}

#endif // KORE_ARRAY_EXPRESSION_HPP
