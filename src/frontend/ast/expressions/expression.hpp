#ifndef KORE_EXPRESSION_HPP
#define KORE_EXPRESSION_HPP

#include <memory>
#include <vector>

#include "ast/ast_node.hpp"
#include "types/type.hpp"
#include "internal_value_types.hpp"
#include "operator.hpp"
#include "token.hpp"

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
            using pointer = std::unique_ptr<Expression>;
            using reference = std::unique_ptr<Expression>&;

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

            static Expression* make_parser_error(const std::string& msg, const SourceLocation&);
            static Expression* make_array_fill(Expression*, Expression*, const SourceLocation&);
            static Expression* make_empty_array();
            static Expression* make_array_range(Expression*, Expression*, const SourceLocation&);
            static Expression* make_bool_literal(const std::string&, const SourceLocation&);
            static Expression* make_int_literal(i32, const SourceLocation&);
            static Expression* make_float_literal(f32, const SourceLocation&);
            static Expression* make_char_literal(i32, const SourceLocation&);
            static Expression* make_string_literal(const std::string&, const SourceLocation&);
            static Identifier* make_identifier(const Token&);
            static Identifier* make_identifier(const std::string&, const SourceLocation&);
            static Identifier* make_identifier(const std::vector<std::string>&, const SourceLocation&);
            static Expression* make_unary(const std::string& op, Expression*, const SourceLocation&);
            static Expression* make_binary(const std::string& op, Expression*, Expression*, const SourceLocation&);
            static Expression* make_parameter(const Token&, Type*);
            static Expression* make_call(Identifier*, std::vector<Expression*>& parameters);

            template<typename T, typename ...Args>
            static Expression* make_expression(Args... args) {
                return std::make_unique<T>(std::forward(args)...);
            }

        protected:
            const Type* _type;

        private:
            ExpressionType _expr_type;
            bool _parenthesised;
    };
}

#endif // KORE_EXPRESSION_HPP
