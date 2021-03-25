#ifndef KORE_TOKEN_TYPE_HPP
#define KORE_TOKEN_TYPE_HPP

#include <map>
#include <ostream>

/// Types of tokens
enum class TokenType {
    arrow,
    assign,
    at,
    bar,
    character,
    colon,
    comma,
    div,
    dot,
    eof,
    equal,
    exp,
    floatp,
    ge,
    gt,
    identifier,
    integer,
    invalid,
    keyword,
    lbrace,
    lbracket,
    le,
    lparen,
    lt,
    minus,
    mult,
    multi_line_comment,
    not_equal,
    plus,
    question_mark,
    range,
    rbrace,
    rbracket,
    rparen,
    single_line_comment,
    string,
};

namespace token_names {
    extern const std::map<TokenType, std::string> mapping;
}

std::ostream& operator<<(std::ostream& os, const TokenType& type);

#endif // KORE_TOKEN_TYPE_HPP
