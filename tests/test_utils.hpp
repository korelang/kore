#ifndef KORE_TEST_UTILS_HPP
#define KORE_TEST_UTILS_HPP

#include "scanner.hpp"

namespace kore {
    void expect_token(
        Token token,
        TokenType type,
        const std::string& value,
        std::size_t lnum,
        std::size_t start,
        std::size_t end
    );
}

#endif
