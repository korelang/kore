#ifndef KORE_INTEGER_EXPRESSION_HPP
#define KORE_INTEGER_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"
#include "types/integer_type.hpp"

namespace kore {
    /// An integer literal
    class IntegerExpression : public Expression {
        public:
            IntegerExpression(i32 value, SourceLocation location);
            virtual ~IntegerExpression();

            i32 value() const noexcept;
            const Type* type() const override;

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            i32 _value;
            IntegerType _type;
    };
}

#endif // KORE_INTEGER_EXPRESSION_HPP
