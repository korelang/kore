#include <iostream>
#include <sstream>

#include "ast/expressions/array_expression.hpp"
#include "ast/expressions/array_fill_expression.hpp"
#include "ast/expressions/index_expression.hpp"
#include "ast/expressions/array_range_expression.hpp"
#include "ast/expressions/bool_expression.hpp"
#include "ast/expressions/call.hpp"
#include "ast/expressions/char_expression.hpp"
#include "ast/expressions/field_access_expression.hpp"
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
#include "types/optional.hpp"
#include "types/type.hpp"
#include "errors/errors.hpp"
#include "operator.hpp"
#include "parser.hpp"

namespace kore {
    const std::string Parser::MUTABLE_PREFIX = "var";

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

                oss //<< std::left
                    << " " << name
                    << " " << token->location().format()
                    << " " << token->type()
                    << " " << token->value();

                const std::string str = oss.str();

                debug_group(group, "%s", &str);
            } else {
                error_group(group, "Attempt to log null current token (%s)", name.c_str());
            }
        }
    }

    void Parser::reset() {
        _module_name = "";
        _failed = false;
        _error_count = 0;
        _did_peek = false;
        _ast = nullptr;
        _args = nullptr;
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

    bool Parser::expect_identifier(const std::string& error_message) {
        if (current_token()->is_identifier()) {
            return true;
        }

        if (error_message.length() > 0) {
            emit_parser_error(error_message.c_str());
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
        // TODO: Save errors in a list as with the other ast visitors
        _failed = true;
        ++_error_count;

        SourceLocation loc = current_token()->location();

        std::cerr << ColorAttribute::Bold << Color::Red
                << "[error:parser("
                << loc.start_line()
                << ","
                << loc.format_columns()
                << ")]"
                << ColorAttribute::Reset
                << " ";

        std::cerr <<  _scanner.source_name() << std::endl;

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
            next_token();
            auto token = current_token();
            /* auto token_type = token->type(); */

            /* if (token_type == TokenType::newline || token_type == TokenType::semicolon) { */
            /*     return; */
            /* } */

            if (valid_statement_start(token)) {
                break;
            }
        }
    }

    std::string Parser::module_name() const {
        return _module_name;
    }

    Owned<Expression> Parser::make_parser_error(const std::string& msg) {
        emit_parser_error(msg.c_str());

        return Expression::make<ParserErrorNode>(msg, current_token()->location());
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

    Owned<Expression> Parser::parse_index_expression(Owned<Expression> indexed_expr) {
        trace_parser("index expression");

        auto expr = parse_expression(operator_base_precedence());

        if (!expect_token_type(TokenType::RightBracket)) {
            emit_parser_error("Expect ']' in index expression");
        }

        auto location = SourceLocation(
            indexed_expr->location(),
            current_token()->location()
        );

        auto index_expr = Expression::make<IndexExpression>(
            std::move(expr),
            std::move(indexed_expr),
            location
        );

        next_token();

        return index_expr;
    }

    Owned<Expression> Parser::parse_field_access_expression(Owned<Expression> expr) {
        trace_parser("field access expression");

        if (!expect_token_type(TokenType::Dot, true)) {
            // TODO: Or return an error expression instead?
            return nullptr;
        }

        Owned<Expression> field = Expression::make<Identifier>(*current_token());

        return Expression::make<FieldAccessExpression>(
            std::move(expr),
            std::move(field)
        );
    }

    void Parser::parse_declaration(Statement* const parent) {
        trace_parser("declaration");

        auto lhs_exprs = parse_lhs_expression_list();

        if (lhs_exprs.empty()) {
            return;
        } else if (lhs_exprs.size() == 1 && lhs_exprs[0]->expr_type() == ExpressionType::Call) {
            add_statement(
                parent,
                Statement::make_statement<ExpressionStatement>(std::move(lhs_exprs[0]))
            );

            return;
        }

        auto rhs_exprs = parse_expression_list();

        if (rhs_exprs.empty()) {
            return;
        }

        add_statement(
            parent,
            Statement::make_statement<VariableAssignment>(
                std::move(lhs_exprs),
                std::move(rhs_exprs)
            )
        );

        /* auto token = current_token(); */

        /* if (!token->is_identifier()) { */
        /*     return; */
        /* } */

        /* bool is_mutable = token->value() == "var"; */

        /* if (is_mutable) { */
        /*     token = next_token(); */
        /* } */

        // TODO: Parse an expression list here to support things like
        //
        // a.lol, b[0], var c = d()
        // var c i32, b bool, d str = 1, false, "hello"
        // var c, b, d = 1, false, "hello"
        //
        // If the token after the first expression is comma, continue parsing
        // as an expression list, otherwise parse as a function call. The lhs
        // expression list can only contain valid targets for an assignment so
        // 1 + 2 = d() is not valid

        /* auto identifier_token = *token; */
        /* token = next_token(); */

        /* if (expect_token_type(TokenType::LeftParenthesis, false)) { */
        /*     if (is_mutable) { */
        /*         emit_parser_error("Function calls cannot be declared variable"); */
        /*         return; */
        /*     } */

        /*     auto identifier = Expression::make_expression<Identifier>(identifier_token); */

        /*     add_statement( */
        /*         parent, */
        /*         Statement::make_statement<ExpressionStatement>( */
        /*             parse_function_call(std::move(identifier)) */
        /*         ) */
        /*     ); */

        /*     return; */
        /* } */

        /* Owned<Expression> lhs_expr; */
        /* Owned<Expression> rhs_expr; */
        /* const Type* decl_type; */
        /* auto identifier = Expression::make_expression<Identifier>(identifier_token); */

        /* if (expect_token_type(TokenType::LeftBracket, true)) { */
        /*     lhs_expr = parse_index_expression(std::move(identifier)); */
        /* } else { */
        /*     lhs_expr = std::move(identifier); */
        /*     decl_type = parse_type(); */
        /* } */

        /* if (!expect_token_type(TokenType::Assign)) { */
        /*     emit_parser_error("Variable declarations must be initialised"); */
        /*     return; */
        /* } */

        /* rhs_expr = parse_expression(operator_base_precedence()); */

        /* add_statement( */
        /*     parent, */
        /*     Statement::make_statement<VariableAssignment>( */
        /*         is_mutable, */
        /*         decl_type, */
        /*         std::move(lhs_expr), */
        /*         std::move(rhs_expr) */
        /*     ) */
        /* ); */
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
                next_token(); // TODO: Remove?
                return;
            }
        } else if (!is_func) {
            emit_parser_error("Expected 'func' keyword for valid function declaration");
            return;
        }

        if (!expect_identifier("Expected function name identifier after 'func'")) {
            next_token();
            return;
        }

        trace_parser("function name");

        const auto func_name(*current_token());
        next_token();

        auto func = Statement::make_statement<Function>(exported, func_name);

        // Create a function type for the function we are about to parse.
        // Parsing will fill it with parameter and return types and we save it
        // in the type cache afterwards
        auto func_type = std::make_unique<FunctionType>();
        func->set_type(func_type.get());

        parse_function_signature(func.get());
        parse_block(func.get());

        Type::set_function_type(std::move(func_type));

        add_statement(parent, std::move(func));
    }

    void Parser::parse_function_signature(Function* const func) {
        trace_parser("function signature");

        if (expect_token_type(TokenType::LeftParenthesis)) {
            if (!expect_token_type(TokenType::RightParenthesis)) {
                // TODO: Return parameter types from this method for constructing the function type
                parse_function_parameters(func);
            }

            // auto return_types_start = current_token()->location();
            auto return_types = parse_type_list();

            if (return_types.empty()) {
                // If no return type was specified, mark it as unknown and
                // infer it in the type inference pass
                func->type()->add_return_type(Type::unknown());
            } else {
                bool has_void_type = false;

                for (auto type : return_types) {
                    if (type->is_void()) {
                        has_void_type = true;
                    }

                    func->type()->add_return_type(type);
                }

                if (return_types.size() > 1 && has_void_type) {
                    // TODO: Allow passing custom source location ranges
                    // TODO: Allow disabling advancing to next statement boundary or skip body
                    emit_parser_error("Function cannot return multiple values where one type is void. Either return void or one or more non-void types");
                }
            }
        }
    }

    void Parser::parse_function_parameters(Function* const func) {
        trace_parser("function parameters");

        parse_parameter_list(func);
    }

    void Parser::parse_parameter_decl(Function* const func) {
        trace_parser("parameter declaration");
        auto token = *current_token();

        if (token.type() == TokenType::Identifier) {
            auto parameter = Expression::make<Parameter>(token, Type::unknown());
            token = *next_token();

            if (token.type() != TokenType::Comma && token.type() != TokenType::RightParenthesis) {
                parameter->set_type(parse_type());
            }

            // TODO: Need to fix this
            func->type()->add_parameter_type(parameter->type());
            func->add_parameter(std::move(parameter));
        } else {
            emit_parser_error("Unexpected token '%s' in function parameter", token.type());
        }
    }

    void Parser::parse_parameter_list(Function* const func) {
        trace_parser("parameter list");

        SourceLocation loc = current_token()->location();

        do {
            parse_parameter_decl(func);
            auto token_type = current_token()->type();

            if (token_type == TokenType::Comma) {
                next_token();
            } else if (token_type == TokenType::RightParenthesis) {
                next_token();
                break;
            } else {
                emit_parser_error(
                    "Expected ',' or ')' after parameter declaration but got unexpected token '%s'",
                    token_type
                );
                break;
            }
        } while (true);
    }

    void Parser::parse_return(Statement* const parent) {
        trace_parser("return");

        auto start_loc = current_token()->location();
        next_token();
        auto expr_list = parse_expression_list(); // TODO: Doesn't work
        add_statement(parent, Statement::make_statement<Return>(std::move(expr_list)));
    }

    IdentifierList Parser::parse_identifier_list() {
        IdentifierList identifiers;
        const Token* token = current_token();
        SourceLocation loc = token->location();

        do {
            if (token->is_identifier()) {
                identifiers.emplace_back(Expression::make<Identifier>(*token));
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

    std::vector<const Type*> Parser::parse_type_list() {
        std::vector<const Type*> types;

        do {
            auto type = parse_type();
            types.push_back(type);

            if (expect_token_type(TokenType::LeftBrace, false)) {
                break;
            } else if (expect_token_type(TokenType::Comma)) {
                // Moves to next token
            } else {
                emit_parser_error("Expected ',' or '{' when parsing function return type");
                break;
            }
        } while (true);

        return types;
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
                result = Expression::make<IntegerExpression>(
                    token->int_value() * sign,
                    token->location()
                );
                next_token();
                break;
            }

            case TokenType::Float: {
                result = Expression::make<FloatExpression>(
                    token->float32_value() * sign,
                    token->location()
                );
                next_token();
                break;
            }

            case TokenType::Character: {
                // TODO: Handle signed characters
                result = Expression::make<CharExpression>(token->int_value(), token->location());
                next_token();
                break;
            }

            case TokenType::String: {
                // TODO: Handle signed strings (an error)
                result = Expression::make<StringExpression>(
                    token->value(),
                    token->location()
                );
                next_token();
                break;
            }

            case TokenType::Keyword: {
                if (token->is_boolean_keyword()) {
                    result = Expression::make<BoolExpression>(token->value(), token->location());
                    next_token();
                } else {
                    result = Expression::make<ParserErrorNode>(
                        "Expected literal token type, got keyword",
                        token->location()
                    );
                }
                break;
            }

            case TokenType::LeftBracket: {
                result = parse_array(*token);
                break;
            }

            default: {
                result = Expression::make<ParserErrorNode>("Expected literal token type", token->location());
                break;
            }
        }

        return result;
    }

    Owned<Expression> Parser::parse_array(const Token lbracket_token) {
        trace_parser("array");

        next_token();
        auto first_expr = parse_expression(operator_base_precedence());
        Owned<Expression> result = nullptr;

        if (expect_token_type(TokenType::Colon)) {
            result = parse_array_fill_expression(lbracket_token, std::move(first_expr));
        } else if (expect_token_type(TokenType::Comma)) {
            result = parse_normal_array_expression(lbracket_token, std::move(first_expr));
        } else {
            emit_parser_error("Expected ':' or ',' in array literal");
        }

        return result;
    }

    Owned<Expression> Parser::parse_array_fill_expression(
        const Token& lbracket_token,
        Owned<Expression> size_expr
    ) {
        trace_parser("array fill");
        auto fill_expr = parse_expression(operator_base_precedence());

        if (!expect_token_type(TokenType::RightBracket)) {
            return make_parser_error("Expected ']' after array fill expression");
        }

        return Expression::make<ArrayFillExpression>(
            std::move(size_expr),
            std::move(fill_expr),
            lbracket_token.location()
        );
    }

    Owned<Expression> Parser::parse_normal_array_expression(
        const Token& lbracket_token,
        Owned<Expression> first_elem_expr
    ) {
        trace_parser("normal array");

        auto array_expr = Expression::make<ArrayExpression>(lbracket_token.location());
        array_expr->add_element(std::move(first_elem_expr));

        while (!_scanner.eof()) {
            auto element_expr = parse_expression(operator_base_precedence());
            array_expr->add_element(std::move(element_expr));

            if (expect_token_type(TokenType::RightBracket, false)) {
                array_expr->set_end_location(current_token()->location());
                next_token();

                break;
            } else if (!expect_token_type(TokenType::Comma)) {
                array_expr->add_element(make_parser_error("Expected ',' after array element expression"));

                return array_expr;
            }
        }

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

        return Expression::make<ArrayRangeExpression>(
            std::move(start_expr),
            std::move(end_expr),
            lbracket_token->location()
        );
    }

    Owned<Identifier> Parser::parse_maybe_qualified_identifier() {
        trace_parser("identifier");

        std::vector<std::string> identifier;
        auto token = current_token();

        do {
            if (token->is_identifier()) {
                identifier.emplace_back(token->value());
                token = next_token();

                if (token->type() != TokenType::Dot) {
                    break;
                }

                token = next_token();
            } else {
                break;
            }
        } while (true);

        return Expression::make<Identifier>(identifier, token->location());
    }

    Owned<Expression> Parser::parse_unary_expression() {
        trace_parser("unary expression");
        auto expr = parse_subexpr();

        if (expr && !expr->is_error()) {
            return expr;
        }

        auto token = current_token();

        if (token->is_keyword() && token->keyword() == Keyword::None) {
            auto expr = Expression::make<Identifier>(*token);
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

        if (!expect_token_type(TokenType::LeftParenthesis)) {
            return {};
        }

        auto expr_list = parse_expression_list();

        return Expression::make<Call>(
            std::move(func_name),
            std::move(expr_list)
        );
    }

    ExpressionList Parser::parse_expression_list() {
        trace_parser("expression list");

        ExpressionList expr_list;

        if (expect_token_type(TokenType::RightParenthesis)) {
            return expr_list;
        }

        Owned<Expression> expr = nullptr;

        while ((expr = parse_expression())) {
            trace_parser("exprlist:expr");
            expr_list.push_back(std::move(expr));

            if (expect_token_type(TokenType::RightParenthesis)) {
                break;
            } else if (!expect_token_type(TokenType::Comma)) {
                break;
            }
        }

        return expr_list;
    }

    std::pair<Owned<Expression>, bool> Parser::parse_lhs_expression() {
        auto token = current_token();
        bool is_mutable = expect_keyword(Keyword::Var);

        if (!token->is_identifier()) {
            emit_parser_error("Invalid left-hand side target for assignment");
            return { nullptr, false };
        }

        auto identifier_token = *token;
        Owned<Expression> expr = Expression::make<Identifier>(identifier_token, is_mutable);

        bool parsed_index_or_field_expr = false;
        next_token();

        // Continue parsing nested field access or index sub-expressions e.g. "a.b[0].c[1]"
        // or we parse a function call e.g. "a.b[0].c[1]()"
        do {
            if (expect_token_type(TokenType::LeftBracket, false)) {
                expr = parse_index_expression(std::move(expr));
                parsed_index_or_field_expr = true;
            } else if (expect_token_type(TokenType::Dot, false)) {
                expr = parse_field_access_expression(std::move(expr));
                parsed_index_or_field_expr = true;
            } else if (expect_token_type(TokenType::LeftParenthesis, false)) {
                if (is_mutable) {
                    emit_parser_error("Function calls cannot be declared variable");
                    return { nullptr, false };
                }

                auto func_call = parse_function_call(std::move(expr));

                return { std::move(func_call), true };
            } else {
                break;
            }
        } while (true);

        if (parsed_index_or_field_expr) {
            if (is_mutable) {
                emit_parser_error("Cannot declare left-hand side index or field access expression as variable");
                return { nullptr, false };
            }
        } else {
            // TODO: Perhaps let parse_type see if the next thing is an identifier instead
            if (!expect_token_type(TokenType::Comma, false) && !expect_token_type(TokenType::Assign, false)) {
                expr->as<Identifier>()->set_declared_type(parse_type());
            }
        }

        return { std::move(expr), false };
    }

    std::vector<Owned<Expression>> Parser::parse_lhs_expression_list() {
        std::vector<Owned<Expression>> lhs_exprs;
        bool contains_function_call = false;

        do {
            auto [lhs_expr, parsed_function_call] = parse_lhs_expression();
            contains_function_call = parsed_function_call;

            if (!lhs_expr) {
                return {};
            }

            lhs_exprs.emplace_back(std::move(lhs_expr));

            if (expect_token_type(TokenType::Assign)) {
                if (contains_function_call) {
                    emit_parser_error("Function call cannot be the target of an assignment");
                }

                return lhs_exprs;
            } else if (parsed_function_call) {
                break;
            } else if (!expect_token_type(TokenType::Comma)) {
                emit_parser_error("Expected ',' or '=' after expression in left-hand side expression list");
                return lhs_exprs;
            }
        } while (true);

        return lhs_exprs;
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
                left = Expression::make<ArrayRangeExpression>(
                    std::move(left),
                    std::move(right),
                    binop_location
                );
            } else {
                auto location = SourceLocation(left->location(), right->location());

                left = Expression::make<BinaryExpression>(
                    op,
                    std::move(left),
                    std::move(right),
                    location
                );
            }
        }

        return left;
    }

    ParseResult Parser::parse_non_module(
        const std::string& value,
        const ParsedCommandLineArgs& args
    ) {
        Ast ast{"<raw>"};

        _ast = &ast;
        _args = &args;
        _scanner.scan_string(value);

        next_token();
        parse_declaration();

        return handle_parse_result(ast);
    }

    ParseResult Parser::parse_string(
        const std::string& value,
        const ParsedCommandLineArgs& args
    ) {
        Ast ast{"<string>"};

        _ast = &ast;
        _args = &args;
        _scanner.scan_string(value);

        next_token();
        parse_toplevel();

        return handle_parse_result(ast);
    }

    ParseResult Parser::parse_file(
        const std::string& path,
        const ParsedCommandLineArgs& args
    ) {
        if (!_scanner.open_file(path)) {
            return {};
        }

        Ast ast{path};
        _ast = &ast;
        _args = &args;

        next_token();
        parse_toplevel();

        return handle_parse_result(ast);
    }

    ParseResult Parser::handle_parse_result(Ast& ast) {
        if (failed()) {
            reset();
            return {};
        }

        ParseResult result = std::make_optional(std::move(ast));
        reset();

        return result;
    }
}
