#include <iomanip>
#include <string>

#include "ast/scanner/token.hpp"

namespace kore {
    Token::Token() : Token(TokenType::Invalid, SourceLocation::unknown) {
    }

    Token::Token(const Token& token)
        : _type(token.type()),
        _location(token.location()),
        _value(token.value()),
        _category(token.category())
    {
        if (_type == TokenType::Integer) {
            _internal_value.integer = token.int_value();
        } else if (_type == TokenType::Float) {
            _internal_value.float32 = token.float32_value();
        } else if (_type == TokenType::Keyword) {
            _internal_value.keyword = token.keyword();
        } else if (_type == TokenType::Character) {
            _internal_value.integer = token.int_value();
        } else {
            _internal_value.str = token.string_value();
        }
    }

    Token::Token(TokenType type, const SourceLocation& location)
        : Token(type, location, "") {
    }

    Token::Token(TokenType type, const SourceLocation& location, const std::string& value)
        : _type(type),
        _location(location),
        _value(value) {
        _category = token_category_from_type(type);
    }

    Token::~Token() {
    }

    Token& Token::operator=(const Token& token) {
        _type = token.type();
        _location = token.location();
        _value = token.value();
        _category = token.category();

        if (_type == TokenType::Integer) {
            _internal_value.integer = token.int_value();
        } else if (_type == TokenType::Float) {
            _internal_value.float32 = token.float32_value();
        } else if (_type == TokenType::Keyword) {
            _internal_value.keyword = token.keyword();
        } else if (_type == TokenType::Character) {
            _internal_value.integer = token.int_value();
        } else {
            _internal_value.str = token.string_value();
        }

        return *this;
    }

    TokenType Token::type() const {
        return _type;
    }

    SourceLocation Token::location() const {
        return _location;
    }

    std::string Token::value() const {
        return _value;
    }

    TokenCategory Token::category() const {
        return _category;
    }

    bool Token::is_eof() const noexcept {
        return _type == TokenType::Eof;
    }

    bool Token::is_identifier() const noexcept {
        return _type == TokenType::Identifier;
    }

    bool Token::is_keyword() const noexcept {
        return _type == TokenType::Keyword;
    }

    bool Token::is_type() const noexcept {
        return is_keyword() && (
            _value == "bool" ||
            _value == "byte" ||
            _value == "char" ||
            _value == "f32"  ||
            _value == "f64"  ||
            _value == "i8"   ||
            _value == "i16"  ||
            _value == "i32"  ||
            _value == "i64"  ||
            _value == "str"  ||
            _value == "u8"   ||
            _value == "u16"  ||
            _value == "u32"  ||
            _value == "u64"
        );
    }

    bool Token::is_boolean_keyword() const noexcept {
        if (type() == TokenType::Keyword) {
            auto kw = keyword();

            return kw == Keyword::True || kw == Keyword::False;
        }

        return false;
    }

    void Token::as_keyword() {
        if (type() == TokenType::Identifier) {
            _type = TokenType::Keyword;
            _internal_value.keyword = keyword_from_string(value());
        }
    }

    i32 Token::int_value() const {
        return _internal_value.integer;
    }

    f32 Token::float32_value() const {
        return _internal_value.float32;
    }

    std::string Token::string_value() const {
        return _internal_value.str;
    }

    Keyword Token::keyword() const {
        return _internal_value.keyword;
    }

    std::string Token::op() const {
        return value();
    }

    bool Token::is_op() const noexcept {
        return operator_precedence(value()) != -1;
    }

    Token Token::make_invalid_token() {
        return Token(TokenType::Invalid, SourceLocation::unknown);
    }

    Token Token::make_eof(std::size_t lnum, std::size_t start, std::size_t end) {
        return Token(TokenType::Eof, SourceLocation(lnum, start, end));
    }

    Token Token::make_token(TokenType type, const SourceLocation& location, const std::string& value) {
        return Token(type, location, value);
    }

    Token Token::make_int_token(
        IntegerFormat format,
        std::size_t lnum,
        std::size_t start,
        std::size_t end,
        const std::string& value
    ) {
        auto token = Token::make_token(TokenType::Integer, SourceLocation(lnum, start, end), value);

        switch (format) {
            case IntegerFormat::dec:
                token._internal_value.integer = std::stoi(value, nullptr, 10);
                break;

            case IntegerFormat::bin:
                token._internal_value.integer = std::stoi(value.substr(2), nullptr, 2);
                break;

            case IntegerFormat::hex:
                token._internal_value.integer = std::stoi(value, nullptr, 16);
                break;

            case IntegerFormat::oct:
                token._internal_value.integer = std::stoi(value, nullptr, 8);
                break;
        }

        return token;
    }

    Token Token::make_char_token(
        codepoint cp,
        std::size_t lnum,
        std::size_t start,
        std::size_t end,
        const std::string& value
    ) {
        auto token = Token::make_token(TokenType::Character, SourceLocation(lnum, start, end), value);
        token._internal_value.integer = cp;

        return token;
    }

    Token Token::make_token(
        TokenType type,
        std::size_t lnum,
        std::size_t start,
        std::size_t end,
        const std::string& value
    ) {
        auto token = Token::make_token(type, SourceLocation(lnum, start, end), value);

        if (type == TokenType::Float) {
            token._internal_value.float32 = std::stof(value);
        } else if (type == TokenType::Keyword) {
            token._internal_value.keyword = keyword_from_string(value);
        }

        return token;
    }

    std::ostream& Token::ostream_value(std::ostream& os) const {
        if (type() == TokenType::Integer) {
            os << _internal_value.integer;
        } else if (type() == TokenType::Float) {
            os << _internal_value.float32;
        } else {
            os << _internal_value.str;
        }

        return os;
    }

    std::ostream& Token::column_format(std::ostream& os, int spacing) const {
        // TODO: Move this to a templated utility function
        return os
            << std::setw(spacing) << std::left << type()
            << std::setw(spacing) << std::left << value()
            << location().colon_format();
    }

    std::ostream& operator<<(std::ostream& os, const Token& token) {
        return os << token.type() << "<" << token.value() << "> at " << token.location();
    }
}
