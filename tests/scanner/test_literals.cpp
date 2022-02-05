#include "catch.hpp"

#include "test_utils.hpp"
#include "scanner.hpp"
#include "location.hpp"

namespace kore {
    TEST_CASE("Can scan literals", "[literals]") {
        Scanner scanner;
        scanner.open_file("./tests/literals.kore");

        expect_token(scanner.next_token(), TokenType::integer, "0xff", 1, 0, 3);
        expect_token(scanner.next_token(), TokenType::integer, "0b1010011", 2, 0, 8);
        expect_token(scanner.next_token(), TokenType::integer, "076314", 3, 0, 5);
        expect_token(scanner.next_token(), TokenType::integer, "1", 4, 0, 0);
        expect_token(scanner.next_token(), TokenType::integer, "10", 5, 0, 1);
        expect_token(
            scanner.next_token(),
            TokenType::single_line_comment,
            "# This is a floating-point number",
            6,
            0,
            32
        );
        expect_token(scanner.next_token(), TokenType::floatp, "10.35", 7, 0, 4);
        expect_token(scanner.next_token(), TokenType::character, "€", 8, 1, 1);
        expect_token(scanner.next_token(), TokenType::string, "\"hello\"", 9, 0, 6);
        expect_token(scanner.next_token(), TokenType::lbracket, "[", 10, 0, 0);
        expect_token(scanner.next_token(), TokenType::integer, "1", 10, 2, 2);
        expect_token(scanner.next_token(), TokenType::comma, ",", 10, 4, 4);
        expect_token(scanner.next_token(), TokenType::integer, "2", 10, 6, 6);
        expect_token(scanner.next_token(), TokenType::comma, ",", 10, 7, 7);
        expect_token(scanner.next_token(), TokenType::integer, "3", 10, 8, 8);
        expect_token(scanner.next_token(), TokenType::rbracket, "]", 10, 10, 10);
        expect_token(scanner.next_token(), TokenType::eof, "", 10, -1, -1);

        REQUIRE(scanner.eof());
    }
}
