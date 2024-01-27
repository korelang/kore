#ifndef KORE_TOKEN_TYPE_HPP
#define KORE_TOKEN_TYPE_HPP

#include <map>
#include <ostream>

namespace kore {
    /// Types of tokens
    enum class TokenType {
        Arrow,
        Assign,
        At,
        Bar,
        Character,
        Colon,
        Comma,
        Div,
        Dot,
        Eof,
        Equal,
        Exp,
        Float,
        GreaterThanEqual,
        GreaterThan,
        Identifier,
        Integer,
        Invalid,
        Keyword,
        LeftBrace,
        LeftBracket,
        LessThanEqual,
        LeftParenthesis,
        LessThan,
        Minus,
        Mult,
        MultiLineComment,
        NotEqual,
        Plus,
        QuestionMark,
        Range,
        RightBrace,
        RightBracket,
        RightParenthesis,
        SingleLineComment,
        String,
    };

    namespace token_names {
        extern const std::map<TokenType, std::string> mapping;
    }

    std::ostream& operator<<(std::ostream& os, const TokenType& type);
}

#endif // KORE_TOKEN_TYPE_HPP
