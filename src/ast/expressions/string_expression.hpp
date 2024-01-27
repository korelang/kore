#ifndef KORE_STRING_EXPRESSION_HPP
#define KORE_STRING_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "internal_value_types.hpp"
#include "types/str_type.hpp"

namespace kore {
    /// A string literal
    class StringExpression : public Expression {
        public:
            StringExpression(const std::string& value, SourceLocation location);
            virtual ~StringExpression();

            std::string value() const noexcept;
            const Type* type() const override;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            std::string _value;
            StrType _type;
    };
}

#endif // KORE_STRING_EXPRESSION_HPP
