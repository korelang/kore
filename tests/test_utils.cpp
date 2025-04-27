#include "test_utils.hpp"
#include "catch.hpp"

namespace kore {
    void expect_token(
        Token token,
        TokenType type,
        const std::string& value,
        int lnum,
        int start,
        int end
    ) {
        REQUIRE(token.type() == type);
        REQUIRE(token.value() == value);

        if (!token.is_eof()) {
            REQUIRE(token.location().start_line() == lnum);
            REQUIRE(token.location().start_col() == start);
            REQUIRE(token.location().end_col() == end);
        }
    }
}
