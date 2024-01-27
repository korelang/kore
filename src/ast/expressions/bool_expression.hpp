#ifndef KORE_BOOL_EXPRESSION_HPP
#define KORE_BOOL_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"
#include "types/bool_type.hpp"

namespace kore {
    class BoolExpression : public Expression {
        public:
            BoolExpression(const std::string& value, const SourceLocation& location);
            virtual ~BoolExpression();

            std::string value() const;
            bool bool_value() const;
            const Type* type() const override;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            std::string _value;
            BoolType _type;
    };
}

#endif // KORE_BOOL_EXPRESSION_HPP
