#include "catch.hpp"

#include "test_utils.hpp"
#include "scanner.hpp"
#include "location.hpp"

namespace kore {
    TEST_CASE("Can scan simple arithmetic", "[arithmetic]") {
        Scanner scanner;
        scanner.open_file("./tests/arith.kore");

        expect_token(scanner.next_token(), TokenType::keyword, "module", 1, 0, 5);
        expect_token(scanner.next_token(), TokenType::identifier, "main", 1, 7, 10);
        expect_token(scanner.next_token(), TokenType::identifier, "a", 3, 0, 0);
        expect_token(scanner.next_token(), TokenType::assign, "=", 3, 2, 2);
        expect_token(scanner.next_token(), TokenType::integer, "1", 3, 4, 4);
        expect_token(scanner.next_token(), TokenType::plus, "+", 3, 6, 6);
        expect_token(scanner.next_token(), TokenType::integer, "2", 3, 8, 8);
        expect_token(scanner.next_token(), TokenType::eof, "", 3, -1, -1);

        REQUIRE(scanner.eof());
    }
}
