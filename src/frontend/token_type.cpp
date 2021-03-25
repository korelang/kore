#include "token_type.hpp"

namespace token_names {
    const std::map<TokenType, std::string> mapping{
        {TokenType::arrow, "arrow"},
        {TokenType::assign, "assign"},
        {TokenType::at, "at"},
        {TokenType::bar, "bar"},
        {TokenType::character, "character"},
        {TokenType::colon, "colon"},
        {TokenType::comma, "comma"},
        {TokenType::div, "div"},
        {TokenType::dot, "dot"},
        {TokenType::eof, "eof"},
        {TokenType::equal, "equal"},
        {TokenType::exp, "exp"},
        {TokenType::floatp, "floatp"},
        {TokenType::ge, "ge"},
        {TokenType::gt, "gt"},
        {TokenType::identifier, "identifier"},
        {TokenType::integer, "integer"},
        {TokenType::invalid, "invalid"},
        {TokenType::keyword , "keyword"},
        {TokenType::lbrace, "lbrace"},
        {TokenType::lbracket, "lbracket"},
        {TokenType::le, "le"},
        {TokenType::lparen, "lparen"},
        {TokenType::lt, "lt"},
        {TokenType::minus, "minus"},
        {TokenType::mult, "mult"},
        {TokenType::multi_line_comment , "multi_line_comment"},
        {TokenType::not_equal, "not_equal"},
        {TokenType::plus, "plus"},
        {TokenType::question_mark, "question_mark"},
        {TokenType::range, "range"},
        {TokenType::rbrace, "rbrace"},
        {TokenType::rbracket, "rbracket"},
        {TokenType::rparen, "rparen"},
        {TokenType::single_line_comment , "single_line_comment"},
        {TokenType::string, "string"},
    };
}

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
    return os << token_names::mapping.at(type);
}
