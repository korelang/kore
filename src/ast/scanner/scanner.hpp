#ifndef KORE_SCANNER_HPP
#define KORE_SCANNER_HPP

#include <filesystem>

#include "pointer_types.hpp"
#include "ast/scanner/token.hpp"

namespace fs = std::filesystem;

namespace kore {
    /// Scanner (or lexer) class that groups up an input stream into tokens
    class Scanner final {
        public:
            /// An iterator for iterating scanned tokens
            class token_iterator final {
                public:
                    using iterator_category = std::input_iterator_tag;
                    using value_type = const Token;
                    using difference_type = std::ptrdiff_t;
                    using pointer = const Token*;
                    using reference = const Token&;

                public:
                    token_iterator(Token token);
                    token_iterator(Scanner& scanner);

                    token_iterator& operator++();
                    reference operator*() const;
                    bool operator==(token_iterator other) const;
                    bool operator!=(token_iterator other) const;

                private:
                    Token _token;
                    Scanner* _scanner;
            };

        public:
            Scanner();
            virtual ~Scanner();

            bool open_file(const fs::path& path);
            void scan_string(const std::string& string);
            std::string source_name() const;
            bool eof() const;
            std::string current_line() const;
            Token next_token();

            token_iterator begin();
            token_iterator end();

        private:
            std::size_t lnum;
            std::size_t last_col;
            std::size_t col;
            std::string _source_name;
            Owned<std::istream> stream;
            std::string line;

            // True if we are at the top-level where scanning starts
            /* bool toplevel; */

            /* bool in_function; */
            /* bool in_string; */

            // Predicate type for consuming a run of characters
            using Predicate = bool (Scanner::*)(char) const;

            void throw_error(const std::string& msg);

            void read_line();
            std::string consume_line();
            Token consume_until(TokenType type, const std::string& value);
            std::size_t consume_while(Predicate predicate);
            /* bool accept(const std::string& run); */
            bool accept_any(const std::string& any_valid);
            bool expect(char byte);
            bool expect_peek(char byte);
            inline bool eol() const;
            void skip_whitespace();

            bool is_whitespace(char byte) const noexcept;
            bool is_digit_start(char byte) const noexcept;
            bool is_digit(char byte) const noexcept;
            bool is_hex_digit(char byte) const noexcept;
            bool is_binary_digit(char byte) const noexcept;
            bool is_octal_digit(char byte) const noexcept;
            bool is_valid_identifier_start(char byte) const noexcept;
            bool is_valid_identifier(char byte) const noexcept;

            Token scan_number();
            Token scan_hex_number();
            Token scan_binary_number();
            Token scan_octal_number();
            Token scan_decimal_number();
            Token scan_identifier();
            Token scan_string();
            /* Token scan_format_string(); */
            Token scan_character();
            Token scan_equal_or_arrow();
            codepoint scan_utf8_encoded_codepoint();
            Token scan_comment();
            Token scan_multiline_comment();
            Token scan_mult_or_exp();
            Token scan_dot_or_range();
            Token scan_op_maybe_equal(TokenType op, TokenType equal_op);
            Token scan_not_equal();

            Token make_one_char_token(TokenType type);
            Token make_inline_token(TokenType type, std::size_t end_col, std::size_t advance = 0);
            Token make_inline_int_token(IntegerFormat format, std::size_t end_col, std::size_t advance = 0);
            Token make_char_token(codepoint cp, std::size_t end_col, std::size_t advance = 0);
            Token make_multiline_token(
                TokenType type,
                std::size_t start_lnum,
                std::size_t start_col,
                /* std::size_t end_lnum, */
                std::size_t end_col
            );
    };
}

#endif // KORE_SCANNER_HPP
