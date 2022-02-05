#ifndef KORE_UTF8_HPP
#define KORE_UTF8_HPP

#include <string>

#include "internal_value_types.hpp"

namespace kore {
    const i32 UNICODE_REPLACEMENT_CHARACTER = 0xfffd;

    enum class DecodeError {
        None,
        InvalidSequence,
        EndOfData,
    };

    i32 utf8_decode_string(const std::string& str, DecodeError& error);

    i32 utf8_decode_string_pos(const std::string& str, std::size_t pos, int& num_bytes, DecodeError& error);
}

#endif // KORE_UTF8_HPP
