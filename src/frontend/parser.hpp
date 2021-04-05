#ifndef KORE_PARSER_HPP
#define KORE_PARSER_HPP

#include "ast/ast.hpp"
#include "ast/expressions/array_expression.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/statements/function.hpp"
#include "ast/statements/statement.hpp"
#include "keywords.hpp"
#include "scanner.hpp"
#include "token.hpp"

class ParserErrorNode;
using IdentifierList = std::vector<Identifier*>;
using ParameterList = std::vector<Identifier*>;

/// The parser. Each parse method is annotated with a comment with the /
/// particular part of the grammar that it handles. It is a recursive descent
/// parser.
///
/// See 'kore.grammer' for the full grammar
class Parser final {
    public:
        Parser();
        virtual ~Parser();

        bool failed() const noexcept;
        int error_count() const noexcept;

        std::string module_name() const;

        /// Parse a program in a string 
        /* Ast parse(const std::string& value); */

        /// Parse a program in a file
        void parse_file(const std::string& path, Ast* ast);

    private:
        std::string _module_name;
        bool _failed;
        int _error_count;
        Scanner _scanner;
        Token _current_token;
        bool _did_peek;
        Token _peek_token;
        Ast* _ast;
        Function* _current_function;

        const Token* current_token();

        const Token* peek_token();

        const Token* next_token();

        bool expect_named_identifier(const std::string& name);

        bool expect_keyword(const Keyword& keyword);

        bool expect_token_type(const TokenType& token_type);

        bool expect_type(const std::string& name);

        void emit_parser_error(const char* const format, ...);

        void set_module_name(const std::string& module_name);

        Expression* make_parser_error(const std::string& msg);
        void add_statement(Statement* statement);

        /// Statement = Declaration | SimpleStmt | IfStmt | ForStmt | Block | ReturnStmt .
        Statement* parse_statement();

        /// ModuleDecl = "module" ModuleName .
        void parse_module();

        /// ImportDecl = "import" ModuleName { "." ModuleName } [ "{" ModuleList "}" ] .
        void parse_import_decl();
        void parse_import_spec();

        bool valid_declaration_start(const Token* const token);

        bool valid_function_start(const Token* const token);

        void parse_declaration();

        ///
        void parse_toplevel();

        /// Function = [ "export" ] "func" FunctionName FuncSignature [ FunctionBody ] .
        void parse_function();

        /// FuncSignature = Parameters [ Type ] .
        void parse_function_signature(Function* const func);

        /// Parameters = "(" [ ParameterList ] ")" .
        void parse_function_parameters(Function* const func);

        /// ParameterDecl = [ IdentifierList ] [ "..." ] Type .
        bool parse_parameter_decl(Function* const func);

        /// ParameterList = ParameterDecl { "," ParameterDecl } .
        void parse_parameter_list(Function* const func);

        /// IdentifierList = Identifier { "," Identifier } .
        std::vector<Identifier*> parse_identifier_list();

        /* Type* parse_type(); */

        /// Block = "{" StatementList "}" .
        void parse_block();

        /// int_lit = decimal_lit | binary_lit | octal_lit | hex_lit .
        Expression* parse_literal();

        /// ArrayDecl = Array | ArrayRange | ArrayFill .
        Expression* parse_array(const Token* const lbracket_token);

        /// ArrayFill = "[" Expression ":" Expression "]" .
        Expression* parse_array_fill_expression(
            const Token* const lbracket_token,
            Expression* size_expr
        );

        /// Array = "[" [ ExpressionList ] "]" .
        Expression* parse_normal_array_expression(
            const Token* const lbracket_token,
            Expression* first_expr
        );

        /// ArrayRange = Expression ".." Expression .
        Expression* parse_array_range_expression(const Token* const lbracket_token);

        Identifier* parse_maybe_qualified_identifier();

        Expression* parse_parenthesised_expression();

        /// UnaryExpr = UnaryOp UnaryExpr .
        Expression* parse_unary_expression();

        /// Expression = UnaryExpr | Expression binary_op Expression .
        /// Expresions are parsed using "precedence climbing"
        Expression* parse_expression(int precedence);
};

#endif // KORE_PARSER_HPP
