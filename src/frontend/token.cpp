#include <string>

#include "token.hpp"

Token::Token() : Token(TokenType::invalid, Location::unknown) {
}

Token::Token(const Token& token)
    : _type(token.type()),
      _location(token.location()),
      _value(token.value()),
      _category(token.category())
{
    if (_type == TokenType::integer) {
        _internal_value.integer = token.int_value();
    } else if (_type == TokenType::floatp) {
        _internal_value.float32 = token.float_value();
    } else if (_type == TokenType::keyword) {
        _internal_value.keyword = token.keyword();
    } else {
        _internal_value.str = token.string_value();
    }
}

Token::Token(TokenType type, const Location& location)
    : Token(type, location, "") {
}

Token::Token(TokenType type, const Location& location, const std::string& value)
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

    if (_type == TokenType::integer) {
        _internal_value.integer = token.int_value();
    } else if (_type == TokenType::floatp) {
        _internal_value.float32 = token.float_value();
    } else if (_type == TokenType::keyword) {
        _internal_value.keyword = token.keyword();
    } else {
        _internal_value.str = token.string_value();
    }

    return *this;
}

TokenType Token::type() const {
    return _type;
}

Location Token::location() const {
    return _location;
}

std::string Token::value() const {
    return _value;
}

TokenCategory Token::category() const {
    return _category;
}

bool Token::is_identifier() const noexcept {
    return _type == TokenType::identifier;
}

bool Token::is_keyword() const noexcept {
    return _type == TokenType::keyword;
}

bool Token::is_type() const noexcept {
bool Token::is_boolean_keyword() const noexcept {
    if (type() == TokenType::keyword) {
        auto kw = keyword();

        return kw == Keyword::True || kw == Keyword::False;
    }

    return false;
}

void Token::as_keyword() {
    if (type() == TokenType::identifier) {
        _type = TokenType::keyword;
        _internal_value.keyword = keyword_from_string(value());
    }
}

i32 Token::int_value() const {
    return _internal_value.integer;
}

f32 Token::float_value() const {
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
    return Token(TokenType::invalid, Location::unknown);
}

Token Token::make_eof(std::size_t lnum, std::size_t start, std::size_t end) {
    return Token(TokenType::eof, Location(lnum, start, end));
}

Token Token::make_token(TokenType type, const Location& location, const std::string& value) {
    return Token(type, location, value);
}

Token Token::make_int_token(
    IntegerFormat format,
    std::size_t lnum,
    std::size_t start,
    std::size_t end,
    const std::string& value
) {
    auto token = Token::make_token(TokenType::integer, Location(lnum, start, end), value);

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

Token Token::make_token(
    TokenType type,
    std::size_t lnum,
    std::size_t start,
    std::size_t end,
    const std::string& value
) {
    auto token = Token::make_token(type, Location(lnum, start, end), value);

    if (type == TokenType::floatp) {
        token._internal_value.float32 = std::stof(value);
    } else if (type == TokenType::keyword) {
        token._internal_value.keyword = keyword_from_string(value);
    }

    return token;
}

std::ostream& Token::ostream_value(std::ostream& os) const {
    if (type() == TokenType::integer) {
        os << _internal_value.str;
    } else if (type() == TokenType::floatp) {
        os << _internal_value.float32;
    } else {
        os << _internal_value.str;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    return os << token.type() << "<" << token.value() << "> at " << token.location();
}
