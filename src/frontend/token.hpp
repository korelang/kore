#ifndef KORE_TOKEN_HPP
#define KORE_TOKEN_HPP

#include <cstdint>
#include <ostream>

#include "integer_format.hpp"
#include "internal_value_types.hpp"
#include "keywords.hpp"
#include "location.hpp"
#include "operator.hpp"
#include "token_type.hpp"
#include "token_category.hpp"

namespace kore {
    /// A token generated by the scanner
    class Token final {
        public:
            Token();
            Token(const Token& token);
            Token(TokenType type, const Location& location);
            Token(TokenType type, const Location& location, const std::string& value);
            virtual ~Token();

            Token& operator=(const Token& token);

            TokenType type() const;
            Location location() const;
            std::string value() const;
            TokenCategory category() const;

            bool is_eof() const noexcept;
            bool is_identifier() const noexcept;
            bool is_keyword() const noexcept;
            bool is_type() const noexcept;
            bool is_boolean_keyword() const noexcept;

            void as_keyword();

            i32 int_value() const;
            f32 float32_value() const;
            std::string string_value() const;
            Keyword keyword() const;
            std::string op() const;
            bool is_op() const noexcept;

            std::ostream& ostream_value(std::ostream& os) const;

            static Token make_invalid_token();
            static Token make_eof(std::size_t lnum, std::size_t start, std::size_t end);
            static Token make_int_token(
                IntegerFormat format,
                std::size_t lnum,
                std::size_t start,
                std::size_t end,
                const std::string& value
            );
            static Token make_char_token(
                codepoint cp,
                std::size_t lnum,
                std::size_t start,
                std::size_t end,
                const std::string& value
            );
            static Token make_token(
                TokenType type,
                const Location& location,
                const std::string& value
            );
            static Token make_token(
                TokenType type,
                std::size_t lnum,
                std::size_t start,
                std::size_t end,
                const std::string& value
            );

        private:
            TokenType _type;
            Location _location;
            std::string _value;
            TokenCategory _category;

        private:
            union TokenValue {
                std::string str;
                i32 integer;
                f32 float32;
                /* f64 float64; */
                Keyword keyword;

                // Union does not contain PODs only so the default constructor
                // is deleted and must be explicitly defined
                TokenValue() : str() {}

                ~TokenValue() {}
            };

            TokenValue _internal_value;
    };

    std::ostream& operator<<(std::ostream& os, const Token& token);
}

#endif // KORE_TOKEN_HPP
