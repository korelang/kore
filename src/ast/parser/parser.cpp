#include <iostream>
#include <sstream>

#include "ast/expressions/array_fill_expression.hpp"
#include "ast/expressions/index_expression.hpp"
#include "ast/expressions/array_range_expression.hpp"
#include "ast/expressions/bool_expression.hpp"
#include "ast/expressions/call.hpp"
#include "ast/expressions/char_expression.hpp"
#include "ast/expressions/float_expression.hpp"
#include "ast/expressions/integer_expression.hpp"
#include "ast/expressions/string_expression.hpp"
#include "ast/parser_error_node.hpp"
#include "ast/statements/expression_statement.hpp"
#include "ast/statements/if_statement.hpp"
#include "ast/statements/import_statement.hpp"
#include "ast/statements/module_statement.hpp"
#include "ast/statements/return_statement.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "logging/color_attributes.hpp"
#include "logging/color.hpp"
#include "logging/logging.hpp"
#include "types/array_type.hpp"
#include "types/function_type.hpp"
#include "types/type.hpp"
#include "types/optional.hpp"
#include "types/unknown_type.hpp"
#include "errors/errors.hpp"
#include "operator.hpp"
#include "parser.hpp"

namespace kore {
    Parser::Parser()
        : _failed(false),
        _error_count(0),
        _did_peek(false),
        _ast(nullptr) {
    }

    Parser::~Parser() {}

    bool Parser::failed() const noexcept {
        return _failed;
    }

    int Parser::error_count() const noexcept {
        return _error_count;
    }

    void Parser::trace_parser(const std::string& name) {
        if (_args && _args->trace == TraceOption::Parse) {
            const std::string group = "parse";
            auto token = current_token();
            
            if (token) {
                std::ostringstream oss;
                oss << *token;

                debug_group(group, "%s: %s", name.c_str(), oss.str().c_str());
            } else {
                error_group(group, "Attempt to log null current token (%s)", name.c_str());
            }
        }
    }

    const Token* Parser::current_token() {
        return &this->_current_token;
    }

    const Token* Parser::peek_token() {
        if (!_did_peek) {
            _peek_token = _scanner.next_token();
            _did_peek = true;
        }

        return &this->_peek_token;
    }

    const Token* Parser::next_token() {
        if (_did_peek) {
            _did_peek = false;
            _current_token = _peek_token;

            return &_current_token;
        }

        do {
            _current_token = _scanner.next_token();
        } while (_current_token.category() == TokenCategory::comment);

        return &_current_token;
    }

    bool Parser::expect_named_identifier(const std::string& name) {
        const Token* const token = current_token();

        if (token->is_identifier() && token->value() == name) {
            next_token();

            return true;
        }

        return false;
    }

    bool Parser::expect_keyword(const Keyword& keyword) {
        auto const token = current_token();

        if (token->is_keyword() && token->keyword() == keyword) {
            next_token();
            return true;
        }

        return false;
    }

    bool Parser::expect_token_type(const TokenType& token_type, bool advance) {
        if (current_token()->type() == token_type) {
            if (advance) {
                next_token();
            }

            return true;
        }

        return false;
    }

    bool Parser::expect_type(const std::string& name) {
        const Token* const token = current_token();

        if (token->type() == TokenType::Keyword && token->value() == name) {
            next_token();
            return true;
        }

        return false;
    }

    void Parser::emit_parser_error(const char* const format, ...) {
        _failed = true;
        ++_error_count;

        SourceLocation loc = current_token()->location();

        std::cerr << ColorAttribute::Bold << Color::Red
                << "[error:parser("
                << loc.lnum()
                << ","
                << loc.format_columns()
                << ")]"
                << ColorAttribute::Reset
                << " ";

        std::cerr << " " << _scanner.source_name() << std::endl;

        va_list args;
        va_start(args, format);
        std::vfprintf(stderr, format, args);
        va_end(args);

        std::cerr
            << std::endl
            << format_error_at_line(_scanner.current_line(), current_token()->location())
            << std::endl;

        advance_to_next_statement_boundary();
    }

    void Parser::set_module_name(const std::string& module_name) {
        _module_name = module_name;
    }

    void Parser::add_statement(Statement* const parent, Owned<Statement> statement) {
        // Use the statement that will contain enclosed statements (such as an if
        // statement containing a brach of statements), otherwise use the top-level
        // ast node
        if (parent) {
            parent->add_statement(std::move(statement));
        } else {
            _ast->add_statement(std::move(statement));
        }
    }

    void Parser::parse_statement(Statement* const parent) {
        trace_parser("statement");

        auto token = current_token();

        if (token->is_keyword()) {
            switch (token->keyword()) {
                case Keyword::Return:
                    parse_return(parent);
                    break;

                case Keyword::If:
                    parse_if_statement(parent);
                    break;

                default:
                    break;
            }
        } else {
            if (valid_declaration_start(token)) {
                parse_declaration(parent);
            } else if (token->type() == TokenType::LeftBrace) {
                parse_block(parent);
            }
        }
    }

    void Parser::parse_statement_list(Statement* const parent) {
        trace_parser("statement list");

        while (valid_statement_start(current_token())) {
            parse_statement(parent);
        }
    }

    void Parser::advance_to_next_statement_boundary() {
        trace_parser("advance to next statement boundary");

        while (!_scanner.eof()) {
            auto token = current_token();
            /* auto token_type = token->type(); */

            /* if (token_type == TokenType::newline || token_type == TokenType::semicolon) { */
            /*     return; */
            /* } */

            if (valid_statement_start(token)) {
                break;
            }

            next_token();
        }
    }

    std::string Parser::module_name() const {
        return _module_name;
    }

    Owned<Expression> Parser::make_parser_error(const std::string& msg) {
        emit_parser_error(msg.c_str());

        return Expression::make_expression<ParserErrorNode>(msg, current_token()->location());
    }

    void Parser::parse_module() {
        trace_parser("module");

        auto token = current_token();

        if (expect_keyword(Keyword::Module)) {
            token = current_token();

            if (token->is_identifier()) {
                /* _ast->set_module_name(token->value()); */
                set_module_name(token->value());
                add_statement(nullptr, Statement::make_statement<ModuleStatement>(*token));
            } else {
                emit_parser_error("Module name must be an identifier");
            }
        } else {
            emit_parser_error("File must start with a module declaration");
        }

        next_token();
    }

    void Parser::parse_import_decl() {
        parse_import_spec();
    }

    void Parser::parse_import_spec() {
        if (current_token()->type() != TokenType::Identifier) {
            emit_parser_error("Expected an identifier after 'import' keyword");
            return;
        }

        auto module_name = parse_maybe_qualified_identifier();
        add_statement(nullptr, Statement::make_statement<ImportStatement>(std::move(module_name)));
    }

    bool Parser::valid_statement_start(const Token* const token) {
        if (token->is_identifier()) {
            return true;
        } else if (token->type() == TokenType::LeftBrace) {
            return true;
        } else if (token->is_keyword()) {
            switch (token->keyword()) {
                case Keyword::Return:
                case Keyword::If:
                    return true;

                default:
                    return false;
            }
        } else if (token->category() == TokenCategory::literal) {
            return true;
        }

        return false;
    }

    bool Parser::valid_declaration_start(const Token* const token) {
        return token->is_identifier();
    }

    bool Parser::valid_function_start(const Token* const token) {
        if (token->is_keyword()) {
            auto value = token->value();

            return value == "export" || value == "func";
        }

        return false;
    }

    /* void Parser::parse_type_alias(Statement* const parent) { */
    /*     // TODO: Handle 'export' keyword */
    /*     auto token = current_token(); */

    /*     if (!token->is_identifier()) { */
    /*         emit_parser_error("Expected identifier after 'type'"); */
    /*         return; */
    /*     } */

    /*     auto identifier = *token; */

    /*     if (!expect_token_type(TokenType::assign)) { */
    /*         emit_parser_error("Type declarations must have a right-hand side"); */
    /*         return; */
    /*     } */

    /*     auto rhs = next_token(); */

    /*     // Type aliases can use user-defined types (i.e. MyStruct) or keywords (i.e. i32) */
    /*     if (!rhs->is_identifier() && !rhs->is_keyword()) { */
    /*         emit_parser_error("Expected identifier or keyword on right-hand side of type alias"); */
    /*         return; */
    /*     } */

    /*     add_statement(parent, Statement::make_type_alias(identifier, rhs)); */
    /* } */

    Owned<Expression> Parser::parse_index_expression(
        const Token* const identifier_token
    ) {
        trace_parser("index expression");

        auto expr = parse_expression(operator_base_precedence());

        if (!expect_token_type(TokenType::RightBracket)) {
            emit_parser_error("Expect ']' in array index expression");
        }

        auto identifier = Expression::make_expression<Identifier>(*identifier_token);
        auto location = SourceLocation(
            identifier_token->location(),
            current_token()->location()
        );

        auto index_expr = Expression::make_expression<IndexExpression>(
            std::move(expr),
            std::move(identifier),
            location
        );

        next_token();

        return index_expr;
    }

    Owned<Expression> Parser::parse_index_expression(Owned<Expression> identifier) {
        trace_parser("index expression");

        auto expr = parse_expression(operator_base_precedence());

        if (!expect_token_type(TokenType::RightBracket)) {
            emit_parser_error("Expect ']' in index expression");
        }

        auto location = SourceLocation(
            identifier->location(),
            current_token()->location()
        );

        auto index_expr = Expression::make_expression<IndexExpression>(
            std::move(expr),
            std::move(identifier),
            location
        );

        next_token();

        return index_expr;
    }

    // TODO: Refactor this function
    void Parser::parse_declaration(Statement* const parent) {
        trace_parser("declaration");

        /* if (expect_keyword(Keyword::Type)) { */
        /*     parse_type_alias(parent); */
        /*     return; */
        /* } */

        auto token = current_token();

        if (!token->is_identifier()) {
            return;
        }

        bool is_mutable = token->value() == "var";

        if (is_mutable) {
            token = next_token();
        }

        auto& identifier_token = *token;
        token = next_token();

        if (expect_token_type(TokenType::LeftParenthesis, false)) {
            if (is_mutable) {
                emit_parser_error("Function calls cannot be declared variable");
                return;
            }

            auto identifier = Expression::make_expression<Identifier>(identifier_token);

            add_statement(
                parent,
                Statement::make_statement<ExpressionStatement>(
                    parse_function_call(std::move(identifier))
                )
            );

            return;
        }

        Owned<Expression> lhs_expr;
        Owned<Expression> rhs_expr;
        const Type* decl_type;
        auto identifier = Expression::make_expression<Identifier>(identifier_token);

        if (expect_token_type(TokenType::LeftBracket, true)) {
            lhs_expr = parse_index_expression(std::move(identifier));
        } else {
            lhs_expr = Expression::make_expression<Identifier>(identifier_token);
            decl_type = parse_type();
            auto type = *token;
        }

        if (!expect_token_type(TokenType::Assign)) {
            emit_parser_error("Variable declarations must be initialised");
            return;
        }

        rhs_expr = parse_expression(operator_base_precedence());

        add_statement(
            parent,
            Statement::make_statement<VariableAssignment>(
                is_mutable,
                decl_type,
                std::move(lhs_expr),
                std::move(rhs_expr)
            )
        );
    }

    void Parser::parse_if_statement(Statement* const parent) {
        trace_parser("if");

        if (expect_keyword(Keyword::If)) {
            auto if_statement = Statement::make_statement<IfStatement>();
            auto condition = parse_expression(operator_base_precedence());

            if (condition->is_error()) {
                advance_to_next_statement_boundary();
                add_statement(parent, std::move(if_statement));
                return;
            }

            parse_block(if_statement.get());
            if_statement->add_branch(std::move(condition));

            // Parse multiple 'else-if' statements
            while (expect_keyword(Keyword::Else)) {
                if (expect_keyword(Keyword::If)) {
                    auto elseif_condition = parse_expression(operator_base_precedence());

                    if (elseif_condition->is_error()) {
                        advance_to_next_statement_boundary();
                        break;
                    }

                    parse_block(if_statement.get());
                    if_statement->add_branch(std::move(elseif_condition));
                } else {
                    parse_block(if_statement.get());
                    if_statement->add_else_branch();
                }
            }

            add_statement(parent, std::move(if_statement));
        }
    }

    void Parser::parse_toplevel(Statement* const parent) {
        /* parse_module(); */

        while (expect_keyword(Keyword::Import)) {
            // TODO: How to require newline after each import?
            parse_import_decl();
        }

        auto token = current_token();

        while (token->type() != TokenType::Eof) {
            if (valid_declaration_start(token)) {
                parse_declaration(parent);
            } else if (valid_function_start(token)) {
                parse_function(parent);
            } else {
                if (token->is_keyword() && token->keyword() == Keyword::Return) {
                    emit_parser_error("Can only return from within a function");
                } else {
                    emit_parser_error("Expected a declaration");
                }

                break;
            }
        }
    }

    void Parser::parse_function(Statement* const parent) {
        trace_parser("function");

        bool exported = expect_keyword(Keyword::Export);
        bool is_func = expect_keyword(Keyword::Func);

        if (exported) {
            if (!is_func) {
                emit_parser_error("Expected 'func' after 'export' keyword");
                next_token();
                return;
            }
        } else if (!is_func) {
            return;
        }

        auto token = current_token();

        if (!token->is_identifier()) {
            emit_parser_error("Expected function name identifier after 'func'");
            next_token();
            return;
        }

        trace_parser("function name");
        const Token func_name(*token);
        next_token();

        auto func = Statement::make_statement<Function>(exported, func_name);

        parse_function_signature(func.get());
        parse_block(func.get());

        add_statement(parent, std::move(func));
    }

    void Parser::parse_function_signature(Function* const func) {
        trace_parser("function signature");

        if (expect_token_type(TokenType::LeftParenthesis)) {
            if (!expect_token_type(TokenType::RightParenthesis)) {
                parse_function_parameters(func);
            }

            auto return_type = parse_type();

            // If no return type was specified, mark it as unknown and infer it
            // in the typechecker
            func->set_return_type(return_type ? return_type : Type::unknown());
        }
    }

    void Parser::parse_function_parameters(Function* const func) {
        trace_parser("function parameters");

        auto token = current_token();

        if (token->is_identifier()) {
            parse_parameter_list(func);
        } else {
            emit_parser_error(
                "Expected identifier or ')' for function parameters, but got '%s'",
                token->value().c_str()
            );
        }
    }

    bool Parser::parse_parameter_decl(Function* const func) {
        trace_parser("parameter declaration");

        auto token = *current_token();

        if (token.type() == TokenType::Identifier) {
            auto parameter = Expression::make_expression<Parameter>(token, Type::unknown());
            token = *next_token();

            if (token.type() != TokenType::Comma && token.type() != TokenType::RightParenthesis) {
                parameter->set_type(parse_type());
            }

            func->add_parameter(std::move(parameter));
        } else if (token.type() == TokenType::Comma) {
            next_token();
        } else if (token.type() == TokenType::RightParenthesis) {
            return false;
        } else {
            emit_parser_error("Unexpected token '%s' in function parameter", token.type());
        }

        return token.type() != TokenType::RightParenthesis;
    }

    void Parser::parse_parameter_list(Function* const func) {
        trace_parser("parameter list");

        auto token = current_token();
        SourceLocation loc = token->location();

        while (!_scanner.eof() && parse_parameter_decl(func));

        if (!expect_token_type(TokenType::RightParenthesis)) {
            emit_parser_error("Expected ')' after function parameters");
        }
    }

    void Parser::parse_return(Statement* const parent) {
        trace_parser("return");

        if (expect_keyword(Keyword::Return)) {
            // TODO: Should be an expression list in the future
            auto expr = parse_expression(operator_base_precedence());

            add_statement(parent, Statement::make_statement<Return>(std::move(expr)));
        }
    }

    IdentifierList Parser::parse_identifier_list() {
        IdentifierList identifiers;
        const Token* token = current_token();
        SourceLocation loc = token->location();

        do {
            if (token->is_identifier()) {
                identifiers.emplace_back(Expression::make_expression<Identifier>(*token));
                token = next_token();
                token = next_token();

                if (token->type() != TokenType::Comma) {
                    break;
                }

                token = next_token();
            } else {
                emit_parser_error("Expected an identifier but got '%s'", token->value().c_str());
                break;
            }
        } while (true);

        return identifiers;
    }

    void Parser::parse_block(Statement* const parent) {
        trace_parser("block");

        if (expect_token_type(TokenType::LeftBrace)) {
            parse_statement_list(parent);

            if (!expect_token_type(TokenType::RightBrace)) {
                emit_parser_error("Expected '}' to close block");
            }
        }
    }

    const Type* Parser::parse_type() {
        trace_parser("type");

        auto token = current_token();

        if (token->is_type()) {
            auto type = Type::from_token(*token);
            next_token();
            ArrayType* array_type = nullptr;

            do {
                if (expect_token_type(TokenType::LeftBracket)) {
                    // Array type (possibly nested)
                    if (!expect_token_type(TokenType::RightBracket)) {
                        emit_parser_error("Expected ']' after '[' in array type declaration");
                        return nullptr;
                    }

                    if (!array_type) {
                        array_type = Type::make_array_type(type);
                    } else {
                        array_type->increase_rank();
                    }
                } else if (expect_token_type(TokenType::QuestionMark)) {
                    return Type::make_optional_type(type);
                } else {
                    return array_type ? array_type : type;
                }
            } while (true);
        }

        return Type::unknown();
    }

    Owned<Expression> Parser::parse_literal() {
        trace_parser("literal");

        auto token = current_token();
        int sign = 1;
        Owned<Expression> result = nullptr;

        if (token->value() == "-" || token->value() == "+") {
            // This is a signed literal
            sign = token->value() == "-" ? -1 : 1;
            token = next_token();
        }

        switch (token->type()) {
            case TokenType::Integer: {
                result = Expression::make_expression<IntegerExpression>(
                    token->int_value() * sign,
                    token->location()
                );
                next_token();
                break;
            }

            case TokenType::Float: {
                result = Expression::make_expression<FloatExpression>(
                    token->float32_value() * sign,
                    token->location()
                );
                next_token();
                break;
            }

            case TokenType::Character: {
                // TODO: Handle signed characters
                result = Expression::make_expression<CharExpression>(token->int_value(), token->location());
                next_token();
                break;
            }

            case TokenType::String: {
                // TODO: Handle signed strings (an error)
                result = Expression::make_expression<StringExpression>(
                    token->value(),
                    token->location()
                );
                next_token();
                break;
            }

            case TokenType::Keyword: {
                if (token->is_boolean_keyword()) {
                    result = Expression::make_expression<BoolExpression>(token->value(), token->location());
                    next_token();
                } else {
                    result = Expression::make_expression<ParserErrorNode>(
                        "Expected literal token type, got keyword",
                        token->location()
                    );
                }
                break;
            }

            case TokenType::LeftBracket: {
                result = parse_array(token);
                break;
            }

            default: {
                result = Expression::make_expression<ParserErrorNode>("Expected literal token type", token->location());
                break;
            }
        }

        return result;
    }

    Owned<Expression> Parser::parse_array(const Token* const lbracket_token) {
        trace_parser("array");

        /* auto& token = *current_token(); */
        next_token();
        auto first_expr = parse_expression(operator_base_precedence());
        Owned<Expression> result = nullptr;

        if (expect_token_type(TokenType::Colon)) {
            result = parse_array_fill_expression(lbracket_token, std::move(first_expr));
        } else if (expect_token_type(TokenType::Comma)) {
            result = parse_normal_array_expression(lbracket_token, std::move(first_expr));
        } else if (expect_token_type(TokenType::RightBracket)) {
            /* result = parse_index_expression(&token); */
        } else {
            emit_parser_error("Expected ':' or ',' in array literal");
        }

        return result;
    }

    Owned<Expression> Parser::parse_array_fill_expression(
        const Token* const lbracket_token,
        Owned<Expression> size_expr
    ) {
        trace_parser("array fill");
        auto fill_expr = parse_expression(operator_base_precedence());

        if (!expect_token_type(TokenType::RightBracket)) {
            return make_parser_error("Expected ']' after array fill expression");
        }

        return Expression::make_expression<ArrayFillExpression>(
            std::move(size_expr),
            std::move(fill_expr),
            lbracket_token->location()
        );
    }

    Owned<Expression> Parser::parse_normal_array_expression(
        const Token* const lbracket_token,
        Owned<Expression> first_elem_expr
    ) {
        trace_parser("normal array");

        auto array_expr = Expression::make_expression<ArrayExpression>();
        array_expr->add_element(std::move(first_elem_expr));
        array_expr->set_start_location(lbracket_token->location());

        while (!_scanner.eof()) {
            auto element_expr = parse_expression(operator_base_precedence());
            array_expr->add_element(std::move(element_expr));

            if (expect_token_type(TokenType::RightBracket, false)) {
                break;
            } else if (!expect_token_type(TokenType::Comma)) {
                array_expr->add_element(make_parser_error("Expected ',' after array element expression"));

                return array_expr;
            }
        }

        array_expr->set_end_location(current_token()->location());
        next_token();

        return array_expr;
    }

    Owned<Expression> Parser::parse_array_range_expression(const Token* const lbracket_token) {
        trace_parser("array range");

        int base_precedence = operator_base_precedence();
        auto start_expr = parse_expression(base_precedence);

        if (!expect_token_type(TokenType::Range)) {
            return make_parser_error("Expected range '..' in array range expression");
        }

        auto end_expr = parse_expression(base_precedence);

        return Expression::make_expression<ArrayRangeExpression>(
            std::move(start_expr),
            std::move(end_expr),
            lbracket_token->location()
        );
    }

    Owned<Identifier> Parser::parse_maybe_qualified_identifier() {
        trace_parser("identifier");

        std::vector<std::string> identifier;
        auto token = current_token();
        SourceLocation loc = token->location();

        do {
            if (token->is_identifier()) {
                identifier.emplace_back(token->value());
                token = next_token();

                if (token->type() != TokenType::Dot) {
                    break;
                }

                token = next_token();
            } else {
                emit_parser_error("Expected a possibly qualified identifier");
                break;
            }
        } while (true);

        return Expression::make_expression<Identifier>(identifier, loc);
    }

    Owned<Expression> Parser::parse_unary_expression() {
        trace_parser("unary expression");
        auto expr = parse_subexpr();

        if (expr && !expr->is_error()) {
            return expr;
        }

        auto token = current_token();

        if (token->is_keyword() && token->keyword() == Keyword::None) {
            auto expr = Expression::make_expression<Identifier>(*token);
            next_token();

            return expr;
        }

        expr = parse_maybe_qualified_identifier();

        /* if (!expr->is_error()) { */
        /*     return expr; */
        /* } */

        if (expect_token_type(TokenType::LeftParenthesis)) {
            return parse_parenthesised_expression();
        } else if (expect_token_type(TokenType::LeftBracket)) {
            return parse_index_expression(std::move(expr));
        }

        return make_parser_error("Expected an unary expression");

        /* if (token->category() != TokenCategory::op) { */
        /*     return make_parser_error("Expected a literal"); */
        /* } */

        /* auto value = token->value(); */

        /* if (token->is_op()) { */
        /*     SourceLocation location = token->location(); */
        /*     auto op = token->op(); */
        /*     this->next_token(); */

        /*     auto expr = parse_literal(); */

        /*     if (expr->is_error()) { */
        /*         return expr; */
        /*     } */

        /*     return Expression::make_unary(op, expr, location); */
        /* } */

        /* return make_parser_error("Expected unary operator"); */
    }

    Owned<Expression> Parser::parse_subexpr() {
        trace_parser("subexpr");
        auto expr = parse_operand();

        if (expr && expr->is_identifier()) {
            auto token = current_token();

            if (token->type() == TokenType::LeftParenthesis) {
                auto call = parse_function_call(std::move(expr));

                if (!call->is_error()) {
                    return call;
                } else {
                    return expr;
                }
            } else if (expect_token_type(TokenType::LeftBracket)) {
                return parse_index_expression(std::move(expr));
            }
        }

        return expr;
    }

    Owned<Expression> Parser::parse_operand() {
        trace_parser("operand");
        auto expr = parse_literal();

        if (expr && !expr->is_error()) {
            return expr;
        }

        return parse_maybe_qualified_identifier();
    }

    Owned<Expression> Parser::parse_function_call(Owned<Expression> func_name) {
        trace_parser("call");

        // TODO: We can probably just return the vector here and use its move constructor
        std::vector<Owned<Expression>> expr_list;
        auto result = parse_expression_list(expr_list);

        if (result && result->is_error()) {
            advance_to_next_statement_boundary();
            return result;
        }

        return Expression::make_expression<Call>(
            std::move(func_name),
            std::move(expr_list)
        );
    }

    Owned<Expression> Parser::parse_expression_list(std::vector<Owned<Expression>>& expr_list) {
        trace_parser("expression list");

        if (!expect_token_type(TokenType::LeftParenthesis)) {
            return nullptr;
        } else if (expect_token_type(TokenType::RightParenthesis)) {
            next_token();
            return nullptr;
        }

        auto precedence = operator_base_precedence();
        Owned<Expression> expr = nullptr;

        while ((expr = parse_expression(precedence))) {
            trace_parser("exprlist:expr");
            expr_list.push_back(std::move(expr));

            if (expect_token_type(TokenType::RightParenthesis)) {
                break;
            } else if (!expect_token_type(TokenType::Comma)) {
                return nullptr;
            }
        }

        return nullptr;
    }

    Owned<Expression> Parser::parse_parenthesised_expression() {
        trace_parser("parenthesised expression");
        Owned<Expression> expr = parse_expression(operator_base_precedence());

        if (expect_token_type(TokenType::RightParenthesis)) {
            expr->set_parenthesised(true);
            return expr;
        }

        return make_parser_error("Expected ')' after expression");
    }

    Owned<Expression> Parser::parse_expression(int precedence) {
        trace_parser("expression");
        auto left = parse_unary_expression();

        if (left->is_error()) {
            return left;
        }

        while (true) {
            auto token = current_token();

            if (token->category() != TokenCategory::bin_op) {
                return left;
            }

            int right_precedence = operator_precedence(token->value());

            if (precedence >= right_precedence) {
                // This is where precedence climbing comes in
                return left;
            }

            SourceLocation binop_location = token->location();
            std::string op = token->op();
            next_token();

            auto right = parse_expression(right_precedence);

            if (op == "..") {
                left = Expression::make_expression<ArrayRangeExpression>(
                    std::move(left),
                    std::move(right),
                    binop_location
                );
            } else {
                auto location = SourceLocation(left->location(), right->location());

                left = Expression::make_expression<BinaryExpression>(
                    op,
                    std::move(left),
                    std::move(right),
                    location
                );
            }
        }

        return left;
    }

    void Parser::parse_non_module(const std::string& value, Ast* ast, const ParsedCommandLineArgs& args) {
        _ast = ast;
        _scanner.scan_string(value);
        parse_declaration(nullptr);
        _ast = nullptr;
        _args = &args;
    }

    void Parser::parse_string(const std::string& value, Ast* ast, const ParsedCommandLineArgs& args) {
        _ast = ast;
        _scanner.scan_string(value);

        next_token();
        parse_toplevel(nullptr);

        _ast = nullptr;
        _args = &args;
    }

    void Parser::parse_file(const std::string& path, Ast* ast, const ParsedCommandLineArgs& args) {
        // TODO: Make this return an ast instead
        if (_scanner.open_file(path)) {
            _ast = ast;
            _args = &args;

            // Get the first token
            next_token();

            parse_toplevel(nullptr);

            // Reset internal parser state
            _ast = nullptr;
        }
    }
}
