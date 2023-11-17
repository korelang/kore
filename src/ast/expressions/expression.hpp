#ifndef KORE_EXPRESSION_HPP
#define KORE_EXPRESSION_HPP

#include <memory>
#include <vector>

#include "internal_value_types.hpp"
#include "pointer_types.hpp"
#include "ast/ast_node.hpp"
#include "ast/scanner/token.hpp"
#include "ast/parser/operator.hpp"
#include "types/type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    class IntegerExpression;
    class Identifier;

    enum class ExpressionType {
        Array,
        Binary,
        Call,
        Identifier,
        Error,
        Literal,
        Parameter,
        Unary,
    };

    std::ostream& operator<<(std::ostream& os, ExpressionType expr_type);

    /// Base class for all expressions
    class Expression : public AstNode {
        public:
            using pointer = Owned<Expression>;
            using reference = Owned<Expression>&;

        public:
            Expression(ExpressionType type, const SourceLocation& location);
            virtual ~Expression();

            virtual bool is_error() const noexcept;
            bool is_literal() const noexcept;
            bool is_identifier() const noexcept;
            ExpressionType expr_type() const;
            // TODO: Should this actually be const Type* const
            virtual const Type* type() const;
            void set_type(const Type* type);

            void set_parenthesised(bool flag);

            template<typename T, typename... Args>
            static Owned<T> make_expression(Args... args) {
                return std::make_unique<T>(std::forward<Args>(args)...);
            }

        protected:
            const Type* _type = Type::unknown();

        private:
            ExpressionType _expr_type;
            bool _parenthesised;
    };
}

#endif // KORE_EXPRESSION_HPP
