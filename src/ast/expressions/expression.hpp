#ifndef KORE_EXPRESSION_HPP
#define KORE_EXPRESSION_HPP

#include <memory>
#include <vector>

#include "pointer_types.hpp"
#include "ast/ast_node.hpp"
#include "types/type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    class IntegerExpression;
    class Identifier;

    enum class ExpressionType {
        Array,
        Binary,
        Call,
        FieldAccess,
        Identifier,
        Index,
        Error,
        Literal,
        Parameter,
        Unary,
    };

    /// Base class for all expressions
    class Expression : public AstNode {
        public:
            using pointer = Owned<Expression>;
            using reference = Owned<Expression>&;

        public:
            Expression(ExpressionType type);
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

            /// Name of the type of the expression
            std::string type_name() const;

            template<typename T, typename... Args>
            static Owned<T> make(Args... args) {
                return std::make_unique<T>(std::forward<Args>(args)...);
            }

            template<typename T>
            T* as() {
                return static_cast<T*>(this);
            }

        protected:
            const Type* _type = Type::unknown();

        private:
            ExpressionType _expr_type;
            bool _parenthesised;
    };

    std::ostream& operator<<(std::ostream& os, const Expression& expr);

    using ExpressionList = std::vector<Owned<Expression>>;
}

#endif // KORE_EXPRESSION_HPP
