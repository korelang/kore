#include "token_type.hpp"

namespace kore {
    namespace token_names {
        const std::map<TokenType, std::string> mapping{
            {TokenType::Arrow, "arrow"},
            {TokenType::Assign, "assign"},
            {TokenType::At, "at"},
            {TokenType::Bar, "bar"},
            {TokenType::Character, "character"},
            {TokenType::Colon, "colon"},
            {TokenType::Comma, "comma"},
            {TokenType::Div, "div"},
            {TokenType::Dot, "dot"},
            {TokenType::Eof, "eof"},
            {TokenType::Equal, "equal"},
            {TokenType::Exp, "exp"},
            {TokenType::Float, "float"},
            {TokenType::GreaterThanEqual, "greater_than_equal"},
            {TokenType::GreaterThan, "greater_than"},
            {TokenType::Identifier, "identifier"},
            {TokenType::Integer, "integer"},
            {TokenType::Invalid, "invalid"},
            {TokenType::Keyword , "keyword"},
            {TokenType::LeftBrace, "left_brace"},
            {TokenType::LeftBracket, "left_bracket"},
            {TokenType::LessThanEqual, "less_than_equal"},
            {TokenType::LeftParenthesis, "left_parenthesis"},
            {TokenType::LessThan, "less_than"},
            {TokenType::Minus, "minus"},
            {TokenType::Mult, "mult"},
            {TokenType::MultiLineComment , "multi_line_comment"},
            {TokenType::NotEqual, "not_equal"},
            {TokenType::Plus, "plus"},
            {TokenType::QuestionMark, "question_mark"},
            {TokenType::Range, "range"},
            {TokenType::RightBrace, "right_brace"},
            {TokenType::RightBracket, "right_bracket"},
            {TokenType::RightParenthesis, "right_parenthesis"},
            {TokenType::SingleLineComment , "single_line_comment"},
            {TokenType::String, "string"},
        };
    }

    std::ostream& operator<<(std::ostream& os, const TokenType& type) {
        return os << token_names::mapping.at(type);
    }
}
