#ifndef KORE_PARSER_HPP
#define KORE_PARSER_HPP

#include <vector>

#include "ast/ast.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/identifier.hpp"
#include "ast/statements/function.hpp"
#include "ast/statements/statement.hpp"
#include "ast/scanner/keywords.hpp"
#include "ast/scanner/scanner.hpp"
#include "ast/scanner/token.hpp"
#include "bin/korec/options.hpp"

namespace kore {
    using ExpressionList = std::vector<Owned<Expression>>;
    using IdentifierList = std::vector<Owned<Identifier>>;
    using ParseResult = std::optional<Ast>;

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

            /// Parse a non-module string without requiring a module declaration
            /// and normal program structure. This is used to parse expressions
            /// and statements given on the command line or in the REPL
            ParseResult parse_non_module(
                const std::string& value,
                const ParsedCommandLineArgs& args
            );

            /// Parse a program in a string 
            ParseResult parse_string(
                const std::string& value,
                const ParsedCommandLineArgs& args
            );

            /// Parse a program in a file
            ParseResult parse_file(
                const std::string& path,
                const ParsedCommandLineArgs& args
            );

        private:
            std::string _module_name;
            bool _failed;
            int _error_count;
            Scanner _scanner;
            Token _current_token;
            bool _did_peek;
            Token _peek_token;
            Ast* _ast;
            const ParsedCommandLineArgs* _args;

            static const std::string MUTABLE_PREFIX;

            void trace_parser(const std::string& name);

            /* std::vector<ParserWarnings> _warnings; */
            /* std::vector<ParserError> _errors; */

            ParseResult handle_parse_result(Ast& ast);

            void reset();

            const Token* current_token();

            const Token* peek_token();

            const Token* next_token();

            bool expect_named_identifier(const std::string& name);

            bool expect_keyword(const Keyword& keyword);

            bool expect_token_type(
                const TokenType& token_type,
                bool advance = true
            );

            bool expect_type(const std::string& name);

            void emit_parser_error(const char* const format, ...);

            Owned<Expression> make_parser_error(const std::string& msg);

            void set_module_name(const std::string& module_name);

            void add_statement(Statement* const parent, Owned<Statement> statement);

            // When we encounter an error, this method is called to advance the
            // parser (and scanner) to the beginning of the next expression.
            // Otherwise, we will continue to parse the erroreous expression which
            // might lead to some unintelligible error messages
            void advance_to_next_statement_boundary();

            /// Statement = Declaration | SimpleStmt | IfStmt | ForStmt | Block | ReturnStmt .
            void parse_statement(Statement* const parent);

            /// StatementList = { Statement ";" } .
            void parse_statement_list(Statement* const parent);

            /// ModuleDecl = "module" ModuleName .
            void parse_module();

            /// ImportDecl = "import" ModuleName { "." ModuleName } [ "{" ModuleList "}" ] .
            void parse_import_decl();
            void parse_import_spec();

            bool valid_statement_start(const Token* const token);

            bool valid_declaration_start(const Token* const token);

            bool valid_function_start(const Token* const token);

            /// TypeAlias = [ "export" ] "type" Identifier "=" Type .
            /* void parse_type_alias(Statement* const parent); */

            Owned<Expression> parse_index_expression(Owned<Expression> index_expr);

            Owned<Expression> parse_field_access_expression(Owned<Expression> expr);

            void parse_declaration(Statement* const parent = nullptr);

            /// IfStmt = "if" [ SimpleStmt ] Block [ "else" ( IfStmt | Block ) ] .
            void parse_if_statement(Statement* const parent);

            /// TopLevelDecl = Declaration | Function .
            void parse_toplevel(Statement* const parent = nullptr);

            /// Function = [ "export" ] "func" FunctionName FuncSignature [ FunctionBody ] .
            void parse_function(Statement* const parent);

            /// FuncSignature = Parameters [ Type ] .
            void parse_function_signature(Function* const func);

            /// Parameters = "(" [ ParameterList ] ")" .
            void parse_function_parameters(Function* const func);

            /// ParameterDecl = [ IdentifierList ] [ "..." ] Type .
            bool parse_parameter_decl(Function* const func);

            /// ParameterList = ParameterDecl { "," ParameterDecl } .
            void parse_parameter_list(Function* const func);

            /// ReturnStmt = "return" [ ExpressionList ] .
            void parse_return(Statement* const parent);

            /// IdentifierList = Identifier { "," Identifier } .
            IdentifierList parse_identifier_list();

            const Type* parse_type();

            std::vector<const Type*> parse_type_list();

            /// Block = "{" StatementList "}" .
            void parse_block(Statement* const parent);

            /// int_lit = decimal_lit | binary_lit | octal_lit | hex_lit .
            Owned<Expression> parse_literal();

            /// ArrayDecl = Array | ArrayRange | ArrayFill .
            Owned<Expression> parse_array(const Token* const lbracket_token);

            /// ArrayFill = "[" Expression ":" Expression "]" .
            Owned<Expression> parse_array_fill_expression(
                const Token* const lbracket_token,
                Owned<Expression> size_expr
            );

            /// Array = "[" [ ExpressionList ] "]" .
            Owned<Expression> parse_normal_array_expression(
                const Token* const lbracket_token,
                Owned<Expression> first_expr
            );

            /// ArrayRange = Expression ".." Expression .
            Owned<Expression> parse_array_range_expression(const Token* const lbracket_token);

            Owned<Identifier> parse_maybe_qualified_identifier();

            Owned<Expression> parse_parenthesised_expression();

            /// UnaryExpr = UnaryOp UnaryExpr .
            Owned<Expression> parse_unary_expression();

            Owned<Expression> parse_subexpr();

            Owned<Expression> parse_operand();

            Owned<Expression> parse_function_call(Owned<Expression> func_name);

            ExpressionList parse_expression_list();

            std::pair<Owned<Expression>, bool> parse_lhs_expression();

            /// Parse a list of expressions that are only valid on the left-hand
            /// side of an assignment
            ExpressionList parse_lhs_expression_list();

            /// Expression = UnaryExpr | Expression binary_op Expression .
            /// Expresions are parsed using "precedence climbing"
            Owned<Expression> parse_expression(int precedence = operator_base_precedence());
    };
}

#endif // KORE_PARSER_HPP
