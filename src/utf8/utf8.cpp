#include "utf8.hpp"

#include <iostream>

namespace kore {
    i32 utf8_decode_string(const std::string& str, DecodeError& error) {
        error = DecodeError::None;

        if (str.length() < 1 || str.length() > 4) {
            return -1;
        }

        using ssz = std::string::size_type;

        i32 codepoint = 0;
        ssz extra = 0;
        unsigned int chr1 = static_cast<unsigned int>(str[0]);

        if (chr1 <= 0x7f) {
            // ASCII character
            codepoint = chr1;
        } else {
            if ((chr1 & 0xe0) == 0xc0) {
                // 2-byte encoding
                codepoint += (chr1 & 0xf) << 6;
                extra = 1;
            } else if ((chr1 & 0xf0) == 0xe0) {
                // 3-byte encoding
                codepoint += (chr1 & 0xf) << 12;
                extra = 2;
            } else if ((chr1 & 0xf8) == 0xf0) {
                // 4-byte encoding
                codepoint += (chr1 & 0x7) << 18;
                extra = 3;
            } else {
                return UNICODE_REPLACEMENT_CHARACTER;
            }
        }

        if (str.length() != 1 + extra) {
            return UNICODE_REPLACEMENT_CHARACTER;
        }

        for (ssz i = 0, shift = (extra - 1) * 6; i < extra; ++i, shift -= 6) {
            auto chr = static_cast<unsigned int>(str[i + 1]);

            if ((chr & 0xc0) != 0x80) {
                return UNICODE_REPLACEMENT_CHARACTER;
            }

            codepoint += (chr & 0x3f) << shift;
        }

        return codepoint;
    }

    i32 utf8_decode_string_pos(const std::string& str, std::size_t pos, int& num_bytes, DecodeError& error) {
        if (str.empty()) {
            error = DecodeError::EndOfData;
            num_bytes = 1;
            return -1;
        }

        i32 codepoint = 0;
        error = DecodeError::None;
        int extra = 0;
        unsigned int chr1 = static_cast<unsigned int>(str[pos]);

        if (chr1 <= 0x7f) {
            // ASCII character
            num_bytes = 1;

            return static_cast<i32>(chr1);
        } else {
            if ((chr1 & 0xe0) == 0xc0) {
                // 2-byte encoding
                codepoint += (chr1 & 0xf) << 6;
                extra = 1;
                num_bytes = 2;
            } else if ((chr1 & 0xf0) == 0xe0) {
                // 3-byte encoding
                codepoint += (chr1 & 0xf) << 12;
                extra = 2;
                num_bytes = 3;
            } else if ((chr1 & 0xf8) == 0xf0) {
                // 4-byte encoding
                codepoint += (chr1 & 0x7) << 18;
                extra = 3;
                num_bytes = 4;
            } else {
                return UNICODE_REPLACEMENT_CHARACTER;
            }
        }

        for (int i = 0, shift = (extra - 1) * 6; i < extra; ++i, shift -= 6) {
            auto byte = static_cast<unsigned int>(str[pos + 1 + i]);

            if ((byte & 0xc0) != 0x80) {
                return UNICODE_REPLACEMENT_CHARACTER;
            }

            codepoint += (byte & 0x3f) << shift;
        }

        return codepoint;
    }
}
