#ifndef KORE_STRING_EXPRESSION_HPP
#define KORE_STRING_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"
#include "types/str_type.hpp"

namespace kore {
    /// A string literal
    class StringExpression : public Expression {
        public:
            StringExpression(const std::string& value, Location location);
            virtual ~StringExpression();

            std::string value() const noexcept;
            const Type* type() const override;

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            std::string _value;
            StrType _type;
    };
}

#endif // KORE_STRING_EXPRESSION_HPP
