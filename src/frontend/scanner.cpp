#include <exception>
#include <sstream>
#include <string>

#include "errors.hpp"
#include "keywords.hpp"
#include "logging/logging.hpp"
#include "scanner.hpp"
#include "utf8/utf8.hpp"

namespace kore {
    Scanner::Scanner()
        : lnum(0),
        last_col(0),
        col(0) {
        /* toplevel(true), */
        /* in_function(false), */
        /* in_string(false) { */
    }

    Scanner::~Scanner() {
    }

    Token Scanner::make_one_char_token(TokenType type) {
        auto token = Token::make_token(
            type,
            lnum,
            col,
            col,
            line.substr(col, 1)
        );

        ++col;

        return token;
    }

    Token Scanner::make_inline_token(TokenType type, std::size_t end_col, std::size_t advance) {
        auto token = Token::make_token(
            type,
            lnum,
            last_col,
            end_col,
            line.substr(last_col, end_col - last_col + 1)
        );

        col += advance;
        last_col = col;

        return token;
    }

    Token Scanner::make_inline_int_token(IntegerFormat format, std::size_t end_col, std::size_t advance) {
        auto token = Token::make_int_token(
            format,
            lnum,
            last_col,
            end_col,
            line.substr(last_col, end_col - last_col + 1)
        );

        col += advance;
        last_col = col;

        return token;
    }

    Token Scanner::make_char_token(codepoint cp, std::size_t end_col, std::size_t advance) {
        auto token = Token::make_char_token(
            cp,
            lnum,
            last_col,
            last_col,
            line.substr(last_col, end_col - last_col + 1)
        );

        col += advance;
        last_col = col;

        return token;
    }

    Token Scanner::make_multiline_token(
        TokenType type,
        std::size_t start_lnum,
        std::size_t start_col,
        /* std::size_t end_lnum, */
        std::size_t end_col
    ) {
        return Token::make_token(
            type,
            start_lnum,
            start_col,
            /* end_lnum, */
            end_col,
            line.substr(start_col, end_col - start_col + 1)
        );
    }

    bool Scanner::open_file(const std::string& path) {
        _source_name = path;
        std::ifstream ifs{ path };
        stream = std::make_unique<std::ifstream>(path);
        read_line();

        return ifs.is_open();
    }

    void Scanner::scan_string(const std::string& string) {
        _source_name = "<string>";
        stream = std::make_unique<std::istringstream>(string);
        read_line();
    }

    std::string Scanner::source_name() const {
        return _source_name;
    }

    void Scanner::throw_error(const std::string& msg) {
        error_group("scanner", "%s", format_error(msg, line, lnum, last_col, col).c_str());

        throw_error_for_line(msg, line, lnum, col, col);
    }

    void Scanner::read_line() {
        if (eof()) {
            return;
        }

        std::string current_line = line;
        std::getline(*stream, line);
        ++lnum;
        last_col = 0;
        col = 0;

        /* if (did_read) { */
        /*     ++lnum; */
        /*     last_col = 0; */
        /*     col = 0; */
        /* } else { */
        /*     // Preserve the last input line for proper error reporting in the */
        /*     // parser. If we don't, then error reporting on the last line of any */
        /*     // input will output using an empty string for the current scanner line */
        /*     // */
        /*     // TODO: Is there a better way to do this? */
        /*     /1* line = current_line; *1/ */
        /* } */
    }

    std::string Scanner::consume_line() {
        auto value = line.substr(col);
        read_line();

        return value;
    }

    Token Scanner::consume_until(TokenType type, const std::string& value) {
        std::size_t start_lnum = lnum;
        std::size_t start_col = col;

        while (true) {
            std::size_t idx = line.find(value);

            if (idx != std::string::npos) {
                return make_multiline_token(
                    type,
                    start_lnum,
                    start_col,
                    /* lnum, */
                    idx
                );
            } else {
                read_line();
            }
        }
    }

    std::size_t Scanner::consume_while(Predicate predicate) {
        std::size_t consumed = 0;

        while (!eol()) {
            char byte = line[col];

            if (is_whitespace(byte)) {
                break;
            }

            if (!(this->*predicate)(byte)) {
                return 0;
            }

            ++col;
            ++consumed;
        }

        return consumed;
    }

    /* bool Scanner::accept(const std::string& run) { */
    /*     for (int i = 0; i < run.length() && !eol(); ++i) { */
    /*         if (line[col + i] != run[i]) { */
    /*             return false; */
    /*         } */
    /*     } */

    /*     if (valid.find(byte) != std::string::npos) { */
    /*         ++col; */
    /*         return true; */
    /*     } */

    /*     return false; */
    /* } */

    bool Scanner::accept_any(const std::string& valid) {
        char byte = line[col];

        if (valid.find(byte) != std::string::npos) {
            ++col;
            return true;
        }

        return false;
    }

    bool Scanner::expect(char byte) {
        return line[col] == byte;
    }

    bool Scanner::expect_peek(char byte) {
        if (!eol()) {
            if (line[col + 1] == byte) {
                ++col;
                return true;
            }
        }

        return false;
    }

    inline bool Scanner::eol() const {
        return col >= line.length();
    }

    bool Scanner::eof() const {
        return stream && stream->eof();
    }

    std::string Scanner::current_line() const {
        return line;
    }

    void Scanner::skip_whitespace() {
        while (!eol()) {
            if (is_whitespace(line[col])) {
                ++col;
            } else {
                last_col = col;
                return;
            }
        }
    }

    bool Scanner::is_whitespace(char byte) const noexcept {
        return byte == ' ' || byte == '\t' || byte == '\r';
    }

    bool Scanner::is_digit_start(char byte) const noexcept {
        return byte >= '1' && byte <= '9';
    }

    bool Scanner::is_digit(char byte) const noexcept {
        return byte == '0' || is_digit_start(byte);
    }

    bool Scanner::is_hex_digit(char byte) const noexcept {
        return is_digit(byte)
            || (byte >= 'a' && byte <= 'f')
            || (byte >= 'A' && byte <= 'F');
    }

    bool Scanner::is_binary_digit(char byte) const noexcept {
        return byte == '0' || byte == '1';
    }

    bool Scanner::is_octal_digit(char byte) const noexcept {
        return byte >= '0' && byte <= '7';
    }

    bool Scanner::is_valid_identifier_start(char byte) const noexcept {
        return byte == '_'
            || (byte >= 'a' && byte <= 'z')
            || (byte >= 'A' && byte <= 'Z');
    }

    bool Scanner::is_valid_identifier(char byte) const noexcept {
        return is_valid_identifier_start(byte) || (byte >= '0' && byte <= '9');
    }

    Token Scanner::scan_number() {
        char byte = line[col];

        if (byte == '0') {
            ++col;

            if (accept_any("xX")) {
                return scan_hex_number();
            } else if (accept_any("bB")) {
                return scan_binary_number();
            } else if (accept_any("01234567")) {
                return scan_octal_number();
            }

            --col;
        }

        return scan_decimal_number();
    }

    Token Scanner::scan_hex_number() {
        if (consume_while(&Scanner::is_hex_digit) < 1) {
            throw_error("Invalid hexadecimal number");
        }

        return make_inline_int_token(IntegerFormat::hex, col-1);
    }

    Token Scanner::scan_binary_number() {
        if (consume_while(&Scanner::is_binary_digit) < 1) {
            throw_error("Invalid binary number");
        }

        return make_inline_int_token(IntegerFormat::bin, col-1);
    }

    Token Scanner::scan_octal_number() {
        if (consume_while(&Scanner::is_octal_digit) < 1) {
            throw_error("Invalid octal number");
        }

        return make_inline_int_token(IntegerFormat::oct, col-1);
    }

    Token Scanner::scan_decimal_number() {
        bool dot = false;

        while (!eol()) {
            char byte = line[col];

            if (byte == '.') {
                if (dot) {
                    throw_error("Invalid decimal number (multiple '.' found)");
                }

                dot = true;
            } else if (!is_digit(byte)) {
                break;
            }

            ++col;
        }

        if (dot) {
            return make_inline_token(TokenType::floatp, col-1);
        } else {
            return make_inline_int_token(IntegerFormat::dec, col-1);
        }
    }

    Token Scanner::scan_identifier() {
        while (!eol()) {
            if (!is_valid_identifier(line[col])) {
                break;
            }

            ++col;
        }

        Token token = make_inline_token(TokenType::identifier, col-1);

        if (is_keyword(token.value())) {
            token.as_keyword();
        }

        return token;
    }

    Token Scanner::scan_string() {
        // Skip first double quote
        ++col;

        while (!eol()) {
            if (expect('"')) {
                auto token = make_inline_token(TokenType::string, col);
                ++col;

                return token;
            } else {
                scan_utf8_encoded_codepoint();
            }
        }

        throw_error("End of string not encountered");

        // Make the compiler happy, even though throw_error will always throw
        return Token();
    }

    /* Token Scanner::scan_format_string() { */
    /*     // Skip first double quote */
    /*     ++col; */

    /*     while (!eol()) { */
    /*         char byte = line[col]; */

    /*         if (byte == '{') { */
    /*             // Beginning of a format */
    /*             Token format_token = next_token(); */

    /*             expect('}'); */

    /*             return make_format_token(format_token); */
    /*         } else if (byte == '"') { */
    /*             return make_inline_token(TokenType::string, col++); */
    /*         } else { */
    /*             scan_utf8_encoded_codepoint(); */
    /*         } */
    /*     } */
    /* } */

    Token Scanner::scan_character() {
        // Skip first single quote
        last_col = ++col;

        codepoint cp = scan_utf8_encoded_codepoint();

        if (expect('\'')) {
            return make_char_token(cp, (col++)-1);
        } else {
            throw_error("Character contains more than one character, should be a string?");
        }

        // Make the compiler happy, even though throw_error will always throw
        return Token();
    }

    codepoint Scanner::scan_utf8_encoded_codepoint() {
        if (col >= line.length()) {
            throw_error("Scanned codepoint at end-of-line");
        }

        int num_bytes = 0;
        DecodeError error = DecodeError::None;
        codepoint cp = utf8_decode_string_pos(line, col, num_bytes, error);

        if (error != DecodeError::None) {
            throw_error("Invalid unicode character");
        }

        col += num_bytes;

        return cp;
    }

    Token Scanner::scan_equal_or_arrow() {
        if (expect_peek('>')) {
            return make_inline_token(TokenType::arrow, col);
        } else if (expect_peek('=')) {
            return make_inline_token(TokenType::equal, col);
        }

        return make_one_char_token(TokenType::assign);
    }

    Token Scanner::scan_comment() {
        if (expect('*')) {
            return scan_multiline_comment();
        }

        auto token = make_inline_token(TokenType::single_line_comment, line.length() - 1);

        // Just read the next line now to skip whitespace on the new line
        read_line();

        return token;
    }

    Token Scanner::scan_mult_or_exp() {
        if (expect_peek('*')) {
            return make_inline_token(TokenType::exp, col, 1);
        }

        return make_one_char_token(TokenType::mult);
    }

    Token Scanner::scan_dot_or_range() {
        if (expect_peek('.')) {
            return make_inline_token(TokenType::range, col, 1);
        }

        return make_one_char_token(TokenType::dot);
    }

    Token Scanner::scan_op_maybe_equal(TokenType op, TokenType equal_op) {
        if (expect('=')) {
            return make_inline_token(equal_op, col);
        }

        return make_one_char_token(op);
    }

    Token Scanner::scan_not_equal() {
        if (!expect_peek('=')) {
            throw_error("Expected '=' after '!'");
        }

        return make_inline_token(TokenType::not_equal, col);
    }

    Token Scanner::scan_multiline_comment() {
        return consume_until(TokenType::multi_line_comment, "*#");
    }

    Token Scanner::next_token() {
        /* if (in_string) { */
        /*     // In the process of scanning a, possibly formatted, string */
        /*     scan_string(); */
        /* } */

        skip_whitespace();

        // If we are at the end of the line but not the file, keep reading lines
        while (eol() && !eof()) {
            read_line();
        }

        skip_whitespace();

        if (eol() && eof()) {
            auto eof_token = Token::make_eof(lnum, col, col);

            // Free the input stream as soon as we hit the eof token. This does not
            // really matter for string inputs but for file inputs we should
            // relinquish its resources as soon as we do not need them anymore
            /* stream.reset(nullptr); */

            return eof_token;
        }

        char byte = line[col];

        if (is_digit(byte)) {
            return scan_number();
        } else if (is_valid_identifier_start(byte)) {
            return scan_identifier();
        }

        switch (byte) {
            case '+': return make_one_char_token(TokenType::plus);
            case '-': return make_one_char_token(TokenType::minus);
            case '*': return scan_mult_or_exp();
            case '/': return make_one_char_token(TokenType::div);
            case '<': return scan_op_maybe_equal(TokenType::lt, TokenType::le);
            case '>': return scan_op_maybe_equal(TokenType::gt, TokenType::ge);
            case '!': return scan_not_equal();
            case '(': return make_one_char_token(TokenType::lparen);
            case ')': return make_one_char_token(TokenType::rparen);
            case '{': return make_one_char_token(TokenType::lbrace);
            case '}': return make_one_char_token(TokenType::rbrace);
            case '[': return make_one_char_token(TokenType::lbracket);
            case ']': return make_one_char_token(TokenType::rbracket);
            case ',': return make_one_char_token(TokenType::comma);
            case '@': return make_one_char_token(TokenType::at);
            case '?': return make_one_char_token(TokenType::question_mark);
            case '|': return make_one_char_token(TokenType::bar);
            case ':': return make_one_char_token(TokenType::colon);
            case '.': return scan_dot_or_range();
            case '#': return scan_comment();
            case '"': return scan_string();
            case '\'': return scan_character();
            case '=': return scan_equal_or_arrow();

            default:
                throw_error("Unknown character in stream '" + std::string(byte, 1) + "'");
        }

        // Make the compiler happy, even though this method will throw and neveer
        // get here
        return Token();
    }
}
