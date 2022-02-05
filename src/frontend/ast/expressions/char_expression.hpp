#ifndef KORE_CHAR_EXPRESSION_HPP
#define KORE_CHAR_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"
#include "types/char_type.hpp"

namespace kore {
    /// An char literal
    class CharExpression : public Expression {
        public:
            CharExpression(i32 value, Location location);
            virtual ~CharExpression();

            i32 value() const noexcept;
            const Type* type() const override;
            void write(AstWriter* const writer) override;

        private:
            i32 _value;
            CharType _type;
    };
}

#endif // KORE_CHAR_EXPRESSION_HPP
