#ifndef KORE_TOKEN_CATEGORY_HPP
#define KORE_TOKEN_CATEGORY_HPP

#include "token_type.hpp"

enum class TokenCategory {
    comment,
    op,
    bin_op,
    literal,
    unary_op,
    other,
};

TokenCategory token_category_from_type(TokenType type);

#endif // KORE_TOKEN_CATEGORY_HPPPP
