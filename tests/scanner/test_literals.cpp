#include "catch.hpp"

#include "test_utils.hpp"
#include "ast/scanner/scanner.hpp"
#include "ast/source_location.hpp"

namespace kore {
    TEST_CASE("Can scan literals", "[literals]") {
        Scanner scanner;
        scanner.open_file("./tests/literals.kore");

        expect_token(scanner.next_token(), TokenType::Integer, "0xff", 1, 0, 3);
        expect_token(scanner.next_token(), TokenType::Integer, "0b1010011", 2, 0, 8);
        expect_token(scanner.next_token(), TokenType::Integer, "076314", 3, 0, 5);
        expect_token(scanner.next_token(), TokenType::Integer, "1", 4, 0, 0);
        expect_token(scanner.next_token(), TokenType::Integer, "10", 5, 0, 1);
        expect_token(
            scanner.next_token(),
            TokenType::SingleLineComment,
            "# This is a floating-point number",
            6,
            0,
            32
        );
        expect_token(scanner.next_token(), TokenType::Float, "10.35", 7, 0, 4);
        expect_token(scanner.next_token(), TokenType::Character, "€", 8, 1, 1);
        expect_token(scanner.next_token(), TokenType::String, "\"hello\"", 9, 0, 6);
        expect_token(scanner.next_token(), TokenType::LeftBracket, "[", 10, 0, 0);
        expect_token(scanner.next_token(), TokenType::Integer, "1", 10, 2, 2);
        expect_token(scanner.next_token(), TokenType::Comma, ",", 10, 4, 4);
        expect_token(scanner.next_token(), TokenType::Integer, "2", 10, 6, 6);
        expect_token(scanner.next_token(), TokenType::Comma, ",", 10, 7, 7);
        expect_token(scanner.next_token(), TokenType::Integer, "3", 10, 8, 8);
        expect_token(scanner.next_token(), TokenType::RightBracket, "]", 10, 10, 10);
        expect_token(scanner.next_token(), TokenType::Eof, "", 10, -1, -1);

        REQUIRE(scanner.eof());
    }
}
