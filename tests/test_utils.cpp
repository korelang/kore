#include "test_utils.hpp"
#include "catch.hpp"
#include "scanner.hpp"

namespace kore {
    void expect_token(
        Token token,
        TokenType type,
        const std::string& value,
        std::size_t lnum,
        std::size_t start,
        std::size_t end
    ) {
        REQUIRE(token.type() == type);
        REQUIRE(token.value() == value);

        if (!token.is_eof()) {
            REQUIRE(token.location().lnum() == lnum);
            REQUIRE(token.location().start() == start);
            REQUIRE(token.location().end() == end);
        }
    }
}
