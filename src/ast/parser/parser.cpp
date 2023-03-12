#include <iostream>
#include <sstream>

#include "ast/expressions/array_fill_expression.hpp"
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
#include "logging/colors.hpp"
#include "logging/logging.hpp"
#include "types/array_type.hpp"
#include "types/function_type.hpp"
#include "types/type.hpp"
#include "types/optional.hpp"
#include "types/unknown_type.hpp"
#include "errors/errors.hpp"
#include "operator.hpp"
#include "parser.hpp"

#ifdef KORE_DEBUG_PARSER
    #define KORE_DEBUG_PARSER_LOG_TOKEN(prefix) {\
        auto token = current_token();\
        \
        if (token) {\
            std::cerr << prefix << ": " << *token << std::endl;\
        } else {\
            std::cerr\
                << "Attempt to log null current token at "\
                << __LINE__\
                << ": " << __FILE__\
                << std::endl;\
        }\
    }
#else
    #define KORE_DEBUG_PARSER_LOG_TOKEN(prefix)
#endif

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

        if (token->type() == TokenType::keyword && token->value() == name) {
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
        KORE_DEBUG_PARSER_LOG_TOKEN("statement")

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
            } else if (token->type() == TokenType::lbrace) {
                parse_block(parent);
            }
        }
    }

    void Parser::parse_statement_list(Statement* const parent) {
        KORE_DEBUG_PARSER_LOG_TOKEN("statement list")

        while (valid_statement_start(current_token())) {
            parse_statement(parent);
        }
    }

    void Parser::advance_to_next_statement_boundary() {
        KORE_DEBUG_PARSER_LOG_TOKEN("advance to next statement boundary")

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
        KORE_DEBUG_PARSER_LOG_TOKEN("module")

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
        if (current_token()->type() != TokenType::identifier) {
            emit_parser_error("Expected an identifier after 'import' keyword");
            return;
        }

        auto module_name = parse_maybe_qualified_identifier();
        add_statement(nullptr, Statement::make_statement<ImportStatement>(std::move(module_name)));
    }

    bool Parser::valid_statement_start(const Token* const token) {
        if (token->is_identifier()) {
            return true;
        } else if (token->type() == TokenType::lbrace) {
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

    void Parser::parse_declaration(Statement* const parent) {
        KORE_DEBUG_PARSER_LOG_TOKEN("declaration")

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

        KORE_DEBUG_PARSER_LOG_TOKEN("identifier")
        auto identifier_token = *token;
        token = next_token();

        if (expect_token_type(TokenType::lparen, false)) {
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

        auto decl_type = parse_type();
        auto type = *token;

        if (!expect_token_type(TokenType::assign)) {
            emit_parser_error("Variable declarations must be initialised");
            return;
        }

        auto expr = parse_expression(operator_base_precedence());

        add_statement(
            parent,
            Statement::make_statement<VariableAssignment>(
                is_mutable,
                identifier_token,
                decl_type,
                std::move(expr)
            )
        );
    }

    void Parser::parse_if_statement(Statement* const parent) {
        KORE_DEBUG_PARSER_LOG_TOKEN("if")

        if (expect_keyword(Keyword::If)) {
            // TODO: This will not be cleaned up if something throws
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
        parse_module();

        while (expect_keyword(Keyword::Import)) {
            // TODO: How to require newline after each import?
            parse_import_decl();
        }

        auto token = current_token();

        while (token->type() != TokenType::eof) {
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
        KORE_DEBUG_PARSER_LOG_TOKEN("function")

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

        KORE_DEBUG_PARSER_LOG_TOKEN("function name")
        const Token func_name(*token);
        next_token();

        auto func = Statement::make_statement<Function>(exported, func_name);

        parse_function_signature(func.get());
        parse_block(func.get());

        add_statement(parent, std::move(func));
    }

    void Parser::parse_function_signature(Function* const func) {
        KORE_DEBUG_PARSER_LOG_TOKEN("function signature")

        if (expect_token_type(TokenType::lparen)) {
            if (!expect_token_type(TokenType::rparen)) {
                parse_function_parameters(func);
            }

            auto return_type = parse_type();

            // If no return type was specified, mark it as unknown and infer it
            // in the typechecker
            func->set_return_type(return_type ? return_type : Type::unknown());
        }
    }

    void Parser::parse_function_parameters(Function* const func) {
        KORE_DEBUG_PARSER_LOG_TOKEN("function parameters")

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
        KORE_DEBUG_PARSER_LOG_TOKEN("parameter declaration")

        auto token = *current_token();

        if (token.type() == TokenType::identifier) {
            auto parameter = Expression::make_expression<Parameter>(token, Type::unknown());
            token = *next_token();

            if (token.type() != TokenType::comma && token.type() != TokenType::rparen) {
                parameter->set_type(parse_type());
            }

            func->add_parameter(std::move(parameter));
        } else if (token.type() == TokenType::comma) {
            next_token();
        } else if (token.type() == TokenType::rparen) {
            return false;
        } else {
            emit_parser_error("Unexpected token '%s' in function parameter", token.type());
        }

        return token.type() != TokenType::rparen;
    }

    void Parser::parse_parameter_list(Function* const func) {
        KORE_DEBUG_PARSER_LOG_TOKEN("parameter list")

        auto token = current_token();
        SourceLocation loc = token->location();

        while (!_scanner.eof() && parse_parameter_decl(func));

        if (!expect_token_type(TokenType::rparen)) {
            emit_parser_error("Expected ')' after function parameters");
        }
    }

    void Parser::parse_return(Statement* const parent) {
        KORE_DEBUG_PARSER_LOG_TOKEN("return")

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

                if (token->type() != TokenType::comma) {
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
        KORE_DEBUG_PARSER_LOG_TOKEN("block")

        if (expect_token_type(TokenType::lbrace)) {
            parse_statement_list(parent);

            if (!expect_token_type(TokenType::rbrace)) {
                emit_parser_error("Expected '}' to close block");
            }
        }
    }

    const Type* Parser::parse_type() {
        KORE_DEBUG_PARSER_LOG_TOKEN("type")

        auto token = current_token();

        if (token->is_type()) {
            auto type = Type::from_token(*token);
            next_token();
            ArrayType* array_type = nullptr;

            do {
                if (expect_token_type(TokenType::lbracket)) {
                    // Array type (possibly nested)
                    if (!expect_token_type(TokenType::rbracket)) {
                        emit_parser_error("Expected ']' after '[' in array type declaration");
                        return nullptr;
                    }

                    if (!array_type) {
                        array_type = Type::make_array_type(type);
                    } else {
                        array_type->increase_rank();
                    }
                } else if (expect_token_type(TokenType::question_mark)) {
                    return Type::make_optional_type(type);
                } else {
                    return array_type ? array_type : type;
                }
            } while (true);
        }

        return Type::unknown();
    }

    Owned<Expression> Parser::parse_literal() {
        KORE_DEBUG_PARSER_LOG_TOKEN("literal")

        auto token = current_token();
        Owned<Expression> result = nullptr;

        switch (token->type()) {
            case TokenType::integer:
                result = Expression::make_expression<IntegerExpression>(token->int_value(), token->location());
                next_token();
                break;

            case TokenType::floatp:
                result = Expression::make_expression<FloatExpression>(token->float32_value(), token->location());
                next_token();
                break;

            case TokenType::character:
                result = Expression::make_expression<CharExpression>(token->int_value(), token->location());
                next_token();
                break;

            case TokenType::string:
                result = Expression::make_expression<StringExpression>(token->value(), token->location());
                next_token();
                break;

            case TokenType::keyword:
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

            case TokenType::lbracket:
                result = parse_array(token);
                break;

            default:
                result = Expression::make_expression<ParserErrorNode>("Expected literal token type", token->location());
                break;
        }

        return result;
    }

    Owned<Expression> Parser::parse_array(const Token* const lbracket_token) {
        KORE_DEBUG_PARSER_LOG_TOKEN("array")

        next_token();
        auto first_expr = parse_expression(operator_base_precedence());
        Owned<Expression> result = nullptr;

        if (expect_token_type(TokenType::colon)) {
            result = parse_array_fill_expression(lbracket_token, std::move(first_expr));
        } else if (expect_token_type(TokenType::comma)) {
            result = parse_normal_array_expression(lbracket_token, std::move(first_expr));
        } else {
            emit_parser_error("Expected ':' or ',' in array literal");
        }

        return result;
    }

    Owned<Expression> Parser::parse_array_fill_expression(
        const Token* const lbracket_token,
        Owned<Expression> size_expr
    ) {
        KORE_DEBUG_PARSER_LOG_TOKEN("array fill")
        auto fill_expr = parse_expression(operator_base_precedence());

        if (!expect_token_type(TokenType::rbracket)) {
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
        KORE_DEBUG_PARSER_LOG_TOKEN("normal array")

        auto array_expr = Expression::make_expression<ArrayExpression>();
        array_expr->add_element(std::move(first_elem_expr));
        array_expr->set_start_location(lbracket_token->location());

        while (!_scanner.eof()) {
            auto element_expr = parse_expression(operator_base_precedence());
            array_expr->add_element(std::move(element_expr));

            if (expect_token_type(TokenType::rbracket)) {
                break;
            } else if (!expect_token_type(TokenType::comma)) {
                array_expr->add_element(make_parser_error("Expected ',' after array element expression"));

                return array_expr;
            }
        }

        array_expr->set_end_location(current_token()->location());

        return array_expr;
    }

    Owned<Expression> Parser::parse_array_range_expression(const Token* const lbracket_token) {
        KORE_DEBUG_PARSER_LOG_TOKEN("normal array")

        int base_precedence = operator_base_precedence();
        auto start_expr = parse_expression(base_precedence);

        if (!expect_token_type(TokenType::range)) {
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
        KORE_DEBUG_PARSER_LOG_TOKEN("identifier")

        std::vector<std::string> identifier;
        auto token = current_token();
        SourceLocation loc = token->location();

        do {
            if (token->is_identifier()) {
                identifier.emplace_back(token->value());
                token = next_token();

                if (token->type() != TokenType::dot) {
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
        KORE_DEBUG_PARSER_LOG_TOKEN("unary expression")
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

        if (!expr->is_error()) {
            return expr;
        }

        if (expect_token_type(TokenType::lparen)) {
            return parse_parenthesised_expression();
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
        KORE_DEBUG_PARSER_LOG_TOKEN("subexpr")
        auto expr = parse_operand();

        if (expr && expr->is_identifier()) {
            if (current_token()->type() != TokenType::lparen) {
                return expr;
            }

            auto call = parse_function_call(std::move(expr));

            if (!call->is_error()) {
                return call;
            }
        }

        return expr;
    }

    Owned<Expression> Parser::parse_operand() {
        KORE_DEBUG_PARSER_LOG_TOKEN("operand")
        auto expr = parse_literal();

        if (expr && !expr->is_error()) {
            return expr;
        }

        return parse_maybe_qualified_identifier();
    }

    Owned<Expression> Parser::parse_function_call(Owned<Expression> func_name) {
        KORE_DEBUG_PARSER_LOG_TOKEN("call");

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
        KORE_DEBUG_PARSER_LOG_TOKEN("exprlist")

        if (!expect_token_type(TokenType::lparen)) {
            return nullptr;
        } else if (expect_token_type(TokenType::rparen)) {
            next_token();
            return nullptr;
        }

        auto precedence = operator_base_precedence();
        Owned<Expression> expr = nullptr;

        while ((expr = parse_expression(precedence))) {
            KORE_DEBUG_PARSER_LOG_TOKEN("exprlist:expr")
            expr_list.push_back(std::move(expr));

            if (expect_token_type(TokenType::rparen)) {
                break;
            } else if (!expect_token_type(TokenType::comma)) {
                return nullptr;
            }
        }

        return nullptr;
    }

    Owned<Expression> Parser::parse_parenthesised_expression() {
        KORE_DEBUG_PARSER_LOG_TOKEN("parenthesised expression")
        Owned<Expression> expr = parse_expression(operator_base_precedence());

        if (expect_token_type(TokenType::rparen)) {
            expr->set_parenthesised(true);
            return expr;
        }

        return make_parser_error("Expected ')' after expression");
    }

    Owned<Expression> Parser::parse_expression(int precedence) {
        KORE_DEBUG_PARSER_LOG_TOKEN("expression")
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

    void Parser::parse_non_module(const std::string& value, Ast* ast) {
        _ast = ast;
        _scanner.scan_string(value);
        parse_declaration(nullptr);
        _ast = nullptr;
    }

    void Parser::parse_string(const std::string& value, Ast* ast) {
        _ast = ast;
        _scanner.scan_string(value);

        next_token();

        parse_toplevel(nullptr);
        _ast = nullptr;
    }

    void Parser::parse_file(const std::string& path, Ast* ast) {
        if (_scanner.open_file(path)) {
            _ast = ast;

            // Get the first token
            next_token();

            parse_toplevel(nullptr);

            // Reset internal parser state
            _ast = nullptr;
        }
    }
}

#undef KORE_DEBUG_PARSER_LOG_TOKEN
