#include "token_category.hpp"

namespace kore {
    TokenCategory token_category_from_type(TokenType type) {
        switch (type) {
            /* case TokenType::identifier: */
            /*     return TokenCategory::identifier; */

            case TokenType::SingleLineComment:
            case TokenType::MultiLineComment:
                return TokenCategory::comment;

            case TokenType::Character:
            case TokenType::Float:
            case TokenType::Integer:
            case TokenType::String:
                return TokenCategory::literal;

            case TokenType::Div:
            case TokenType::Equal:
            case TokenType::Exp:
            case TokenType::GreaterThanEqual:
            case TokenType::GreaterThan:
            case TokenType::LessThanEqual:
            case TokenType::LessThan:
            case TokenType::Minus:
            case TokenType::Mult:
            case TokenType::NotEqual:
            case TokenType::Plus:
            case TokenType::Range:
                return TokenCategory::bin_op;

            default:
                return TokenCategory::other;
        }
    }
}
