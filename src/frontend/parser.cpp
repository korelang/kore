#include <iostream>
#include <sstream>

#include "ast/parser_error_node.hpp"
#include "ast/statements/if_statement.hpp"
#include "ast/statements/return_statement.hpp"
#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"
#include "logging/logging.hpp"
#include "types/array_type.hpp"
#include "types/type.hpp"
#include "types/optional.hpp"
#include "types/unknown_type.hpp"
#include "errors.hpp"
#include "operator.hpp"
#include "parser.hpp"

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

bool Parser::expect_token_type(const TokenType& token_type) {
    if (current_token()->type() == token_type) {
        next_token();
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

    Location loc = current_token()->location();

    std::cerr << ColorAttribute::Bold << Color::Red
              << "[error:parser("
              << loc.lnum()
              << ","
              << loc.format_columns()
              << ")]"
              << ColorAttribute::Reset
              << " ";

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

void Parser::add_statement(Statement* const parent, Statement* statement) {
    // Use the statement that will contain enclosed statements (such as an if
    // statement containing a brach of statements), otherwise use the top-level
    // ast node
    if (parent) {
        parent->add_statement(statement);
    } else {
        _ast->add_statement(statement);
    }
}

void Parser::parse_statement(Statement* const parent) {
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
    while (valid_statement_start(current_token())) {
        parse_statement(parent);
    }
}

void Parser::advance_to_next_statement_boundary() {
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

Expression* Parser::make_parser_error(const std::string& msg) {
    emit_parser_error(msg.c_str());

    return Expression::make_parser_error(msg, current_token()->location());
}

void Parser::parse_module() {
    auto token = current_token();

    if (expect_keyword(Keyword::Module)) {
        token = current_token();

        if (token->is_identifier()) {
            set_module_name(token->value());
            add_statement(nullptr, Statement::make_module_decl(*token));
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

    Identifier* module_name = parse_maybe_qualified_identifier();
    add_statement(nullptr, Statement::make_import_decl(module_name));
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

void Parser::parse_declaration(Statement* const parent) {
    auto token = current_token();

    if (!token->is_identifier()) {
        return;
    }

    auto identifier = *token;
    token = next_token();
    Type* decl_type = parse_type();
    auto type = *token;

    if (!expect_token_type(TokenType::assign)) {
        emit_parser_error("Variable declarations must be initialised");
        return;
    }

    Expression* expr = parse_expression(operator_base_precedence());

    add_statement(parent, Statement::make_variable_assignment(identifier, decl_type, expr));
}

void Parser::parse_if_statement(Statement* const parent) {
    if (expect_keyword(Keyword::If)) {
        // TODO: This will not be cleaned up if something throws
        auto if_statement = new IfStatement();
        auto condition = parse_expression(operator_base_precedence());

        if (condition->is_error()) {
            advance_to_next_statement_boundary();
            add_statement(parent, if_statement);
            return;
        }

        parse_block(if_statement);
        if_statement->add_branch(condition);

        if (!expect_keyword(Keyword::Else)) {
            add_statement(parent, if_statement);
            return;
        }

        // Parse multiple 'else-if' statements
        while (expect_keyword(Keyword::If)) {
            auto elseif_condition = parse_expression(operator_base_precedence());

            if (elseif_condition->is_error()) {
                advance_to_next_statement_boundary();
                break;
            }

            parse_block(if_statement);
            if_statement->add_branch(elseif_condition);
        }

        if (expect_keyword(Keyword::Else)) {
            parse_block(if_statement);
            if_statement->add_else_branch();
        }

        add_statement(parent, if_statement);
    }
}

/* void Parser::parse_type_alias() { */
/*     const Token* token = current_token(); */

/*     if (!token->is_identifier()) { */
/*         emit_parser_error("Expected identifier after 'type'"); */
/*         return; */
/*     } */
/* } */

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

    const Token func_name(*token);
    next_token();

    Function* func = Statement::make_function(exported, func_name);

    if (!func) {
        // TODO
    }

    parse_function_signature(func);
    parse_block(func);

    add_statement(parent, func);
}

void Parser::parse_function_signature(Function* const func) {
    if (expect_token_type(TokenType::lparen)) {
        if (expect_token_type(TokenType::rparen)) {
            // No parameters, just return
            return;
        }

        parse_function_parameters(func);

        Type* return_type = parse_type();

        // If no return type was specified, mark it as unknown and infer it in
        // the typechecker
        func->set_return_type(return_type ? return_type : new UnknownType());
    }
}

void Parser::parse_function_parameters(Function* const func) {
    const Token* token = current_token();

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
    auto token = current_token();

    if (token->type() == TokenType::identifier) {
        func->add_parameter(Expression::make_identifier(*token));
        token = next_token();

        if (token->is_type()) {
            // TODO: Parse type
            token = next_token();
        }

        if (token->type() == TokenType::comma) {
            next_token();
        } else if (token->type() == TokenType::rparen) {
            return false;
        } else {
            emit_parser_error("Unexpected token '%s' in function parameter", token->type());
            return false;
        }
    }

    return true;
}

void Parser::parse_parameter_list(Function* const func) {
    auto token = current_token();
    Location loc = token->location();

    while (!_scanner.eof() && parse_parameter_decl(func));

    if (!expect_token_type(TokenType::rparen)) {
        emit_parser_error("Expected ')' after function parameters");
    }
}

void Parser::parse_return(Statement* const parent) {
    if (expect_keyword(Keyword::Return)) {
        // TODO: Should be an expression list in the future
        auto expr = parse_expression(operator_base_precedence());

        add_statement(parent, Statement::make_return(expr));
    }
}

IdentifierList Parser::parse_identifier_list() {
    IdentifierList identifiers;
    const Token* token = current_token();
    Location loc = token->location();

    do {
        if (token->is_identifier()) {
            identifiers.emplace_back(Expression::make_identifier(*token));
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
    if (expect_token_type(TokenType::lbrace)) {
        parse_statement_list(parent);

        if (!expect_token_type(TokenType::rbrace)) {
            emit_parser_error("Expected '}' to close block");
        }
    }
}

Type* Parser::parse_type() {
    auto token = current_token();

    if (token->is_type()) {
        Type* type = Type::from_token(*token);
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
                    array_type = new ArrayType(type);
                } else {
                    array_type->increase_rank();
                }
            } else if (expect_token_type(TokenType::question_mark)) {
                return new Optional(type);
            } else {
                return array_type ? array_type : type;
            }
        } while (true);
    }

    return new UnknownType();
}

Expression* Parser::parse_literal() {
    auto token = current_token();
    Expression* result = nullptr;

    switch (token->type()) {
        case TokenType::integer:
            result = Expression::make_int_literal(token->int_value(), token->location());
            next_token();
            break;

        case TokenType::floatp:
            result = Expression::make_float_literal(token->float32_value(), token->location());
            next_token();
            break;

        case TokenType::character:
            result = Expression::make_char_literal(token->int_value(), token->location());
            next_token();
            break;

        case TokenType::string:
            result = Expression::make_string_literal(token->value(), token->location());
            next_token();
            break;

        case TokenType::keyword:
            if (token->is_boolean_keyword()) {
                result = Expression::make_bool_literal(token->value(), token->location());
                next_token();
            } else {
                result = Expression::make_parser_error(
                    "Expected literal token type, got keyword",
                    token->location()
                );
            }
            break;

        case TokenType::lbracket:
            result = parse_array(token);
            break;

        default:
            result = Expression::make_parser_error("Expected literal token type", token->location());
            break;
    }

    return result;
}

Expression* Parser::parse_array(const Token* const lbracket_token) {
    next_token();
    auto first_expr = parse_expression(operator_base_precedence());
    Expression* result = nullptr;

    if (expect_token_type(TokenType::colon)) {
        result = parse_array_fill_expression(lbracket_token, first_expr);
    } else if (expect_token_type(TokenType::comma)) {
        result = parse_normal_array_expression(lbracket_token, first_expr);
    } else {
        emit_parser_error("Expected ':' or ',' in array literal");
    }

    return result;
}

Expression* Parser::parse_array_fill_expression(
    const Token* const lbracket_token,
    Expression* size_expr
) {
    Expression* fill_expr = parse_expression(operator_base_precedence());

    if (!expect_token_type(TokenType::rbracket)) {
        return make_parser_error("Expected ']' after array fill expression");
    }

    return Expression::make_array_fill(size_expr, fill_expr, lbracket_token->location());
}

Expression* Parser::parse_normal_array_expression(
    const Token* const lbracket_token,
    Expression* first_elem_expr
) {
    ArrayExpression* array_expr = dynamic_cast<ArrayExpression*>(Expression::make_empty_array());
    array_expr->add_element(first_elem_expr);
    array_expr->set_start_location(lbracket_token->location());

    while (!_scanner.eof()) {
        array_expr->add_element(parse_expression(operator_base_precedence()));

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

Expression* Parser::parse_array_range_expression(const Token* const lbracket_token) {
    int base_precedence = operator_base_precedence();
    auto start_expr = parse_expression(base_precedence);

    if (!expect_token_type(TokenType::range)) {
        return make_parser_error("Expected range '..' in array range expression");
    }

    auto end_expr = parse_expression(base_precedence);

    return Expression::make_array_range(start_expr, end_expr, lbracket_token->location());
}

Identifier* Parser::parse_maybe_qualified_identifier() {
    std::vector<std::string> identifier;
    auto token = current_token();
    Location loc = token->location();

    do {
        if (token->is_identifier()) {
            identifier.emplace_back(token->value());
            token = next_token();

            if (token->type() != TokenType::dot) {
                break;
            }

            token = next_token();
        } else {
            emit_parser_error("Expected an possibly qualified identifier");
            break;
        }
    } while (true);

    return Expression::make_identifier(identifier, loc);
}

Expression* Parser::parse_unary_expression() {
    Expression* expr = parse_literal();

    if (!expr->is_error()) {
        return expr;
    }

    auto token = current_token();

    if (token->is_keyword() && token->keyword() == Keyword::None) {
        auto expr = Expression::make_identifier(*token);
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
    /*     Location location = token->location(); */
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

Expression* Parser::parse_parenthesised_expression() {
    Expression* expr = parse_expression(operator_base_precedence());

    if (expect_token_type(TokenType::rparen)) {
        expr->set_parenthesised(true);
        return expr;
    }

    return make_parser_error("Expected ')' after expression");
}

Expression* Parser::parse_expression(int precedence) {
    Expression* left = parse_unary_expression();

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

        Location binop_location = token->location();
        std::string op = token->op();
        next_token();

        auto right = parse_expression(right_precedence);

        if (op == "..") {
            left = Expression::make_array_range(left, right, binop_location);
        } else {
            left = Expression::make_binary(op, left, right, binop_location);
        }
    }

    return left;
}

/* Ast Parser::parse(const std::string& path) { */
/*     return Ast(); */
/* } */

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
