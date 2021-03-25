#include "token_category.hpp"

TokenCategory token_category_from_type(TokenType type) {
    switch (type) {
        /* case TokenType::identifier: */
        /*     return TokenCategory::identifier; */

        case TokenType::single_line_comment:
        case TokenType::multi_line_comment:
            return TokenCategory::comment;

        case TokenType::character:
        case TokenType::floatp:
        case TokenType::integer:
        case TokenType::string:
            return TokenCategory::literal;

        case TokenType::div:
        case TokenType::equal:
        case TokenType::exp:
        case TokenType::ge:
        case TokenType::gt:
        case TokenType::le:
        case TokenType::lt:
        case TokenType::minus:
        case TokenType::mult:
        case TokenType::not_equal:
        case TokenType::plus:
            return TokenCategory::bin_op;

        default:
            return TokenCategory::other;
    }
}
