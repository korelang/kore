#ifndef KORE_PARSER_HPP
#define KORE_PARSER_HPP

#include "ast/ast.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/statements/statement.hpp"
#include "keywords.hpp"
#include "scanner.hpp"
#include "token.hpp"

class ParserErrorNode;

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

        std::string module_name() const;

        /// Parse a program in a string 
        /* Ast parse(const std::string& value); */

        /// Parse a program in a file
        void parse_file(const std::string& path, Ast* ast);

    private:
        std::string _module_name;
        bool _failed;
        Scanner _scanner;
        Token _current_token;
        bool _did_peek;
        Token _peek_token;
        Ast* _ast;

        Token* current_token();

        Token* peek_token();

        Token* next_token();

        bool expect_named_identifier(const std::string& name);

        bool expect_keyword(const Keyword& keyword);

        bool expect_token_type(const TokenType& token_type);

        void emit_parser_error(const char* const format, ...);

        void set_module_name(const std::string& module_name);

        Expression* make_parser_error(const std::string& msg);

        /// Statement = Declaration | SimpleStmt | IfStmt | ForStmt | Block | ReturnStmt .
        Statement* parse_statement();

        /// ModuleDecl = "module" ModuleName .
        void parse_module();

        /// ImportDecl = "import" ModuleName { "." ModuleName } [ "{" ModuleList "}" ] .
        void parse_import_decl();
        void parse_import_spec();

        bool valid_declaration_start(const Token* const token);

        void parse_declaration();

        ///
        void parse_toplevel();

        /// Function = [ "export" ] "func" FunctionName FuncSignature [ FunctionBody ] .
        void parse_function();

        /// int_lit = decimal_lit | binary_lit | octal_lit | hex_lit .
        Expression* parse_literal();

        Expression* parse_maybe_qualified_identifier();

        Expression* parse_parenthesised_expression();

        /// UnaryExpr = UnaryOp UnaryExpr .
        Expression* parse_unary_expression();

        /// Expression = UnaryExpr | Expression binary_op Expression .
        /// Expresions are parsed using "precedence climbing"
        Expression* parse_expression(int precedence);
};

#endif // KORE_PARSER_HPP
