#ifndef KORE_FLOAT_EXPRESSION_HPP
#define KORE_FLOAT_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"
#include "types/float_type.hpp"

namespace kore {
    /// A float literal
    class FloatExpression : public Expression {
        public:
            FloatExpression(f32 value, Location location);
            virtual ~FloatExpression();

            const Type* type() const override;
            f32 value() const noexcept;

            void write(AstWriter* const writer) override;

        private:
            f32 _value;
            FloatType _type{32};
    };
}

#endif // KORE_FLOAT_EXPRESSION_HPP
