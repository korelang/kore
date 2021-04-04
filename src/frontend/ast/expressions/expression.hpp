#ifndef KORE_EXPRESSION_HPP
#define KORE_EXPRESSION_HPP

#include <memory>
#include <vector>

#include "ast/ast_node.hpp"
#include "internal_value_types.hpp"
#include "operator.hpp"
#include "token.hpp"
#include "type.hpp"

class IntegerExpression;
class Identifier;

enum class ExpressionType {
    array,
    binary,
    error,
    identifier,
    literal,
    unary,
};

/// Base class for all expressions
class Expression : public AstNode {
    public:
        // Internal type for subexpressions
        using pointer = std::unique_ptr<Expression>;
        using reference = std::unique_ptr<Expression>&;

    public:
        Expression(ExpressionType type, const Location& location);
        virtual ~Expression();

        virtual bool is_error() const noexcept;
        Type* type() const;

        void set_parenthesised(bool flag);

        /// Wrap this expression as a statement
        /// TODO: Give an example
        /* Statement* as_statement() const; */

        static Expression* make_parser_error(const std::string& msg, const Location&);
        static Expression* make_array_fill(Expression*, Expression*, const Location&);
        static Expression* make_empty_array();
        static Expression* make_array_range(Expression*, Expression*, const Location&);
        static Expression* make_bool_literal(const std::string&, const Location&);
        static Expression* make_int_literal(i32, const Location&);
        static Expression* make_float_literal(f32, const Location&);
        static Expression* make_char_literal(i32, const Location&);
        static Expression* make_string_literal(const std::string&, const Location&);
        static Identifier* make_identifier(const Token&);
        static Identifier* make_identifier(const std::string&, const Location&);
        static Identifier* make_identifier(const std::vector<std::string>&, const Location&);
        static Expression* make_unary(const std::string& op, Expression*, const Location&);
        static Expression* make_binary(const std::string& op, Expression*, Expression*, const Location&);

        template<typename T, typename ...Args>
        static Expression* make_expression(Args... args) {
            return std::make_unique<T>(std::forward(args)...);
        }

    private:
        ExpressionType _type;
        bool _parenthesised;
};

#endif // KORE_EXPRESSION_HPP
