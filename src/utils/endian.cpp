#include <array>
#include <istream>

#include "endian.hpp"

namespace kore {
    bool is_big_endian() {
        const std::uint32_t one = 1;

        return *(std::uint8_t*)(&one) != 1;
    }

    // TODO: Template these functions
    std::uint16_t read_be16(std::istream& is) {
        std::array<char, 2> data;
        is.read(data.data(), 2);

        return (data[0] & 0xff) << 8 | (data[1] & 0xff);

        if (is_big_endian()) {
            return (data[1] & 0xff) << 8 | (data[0] & 0xff);
        } else {
            return (data[0] & 0xff) << 8 | (data[1] & 0xff);
        }
    }

    std::uint32_t read_be32(std::istream& is) {
        std::array<char, 4> data;
        is.read(data.data(), 4);

        return (data[0] & 0xff) << 24 | (data[1] & 0xff) << 16 | (data[2] & 0xff) << 8 | (data[3] & 0xff);

        if (is_big_endian()) {
            return (data[0] & 0xff) << 24 | (data[1] & 0xff) << 16 | (data[2] & 0xff) << 8 | (data[3] & 0xff);
        } else {
            return (data[3] & 0xff) << 24 | (data[2] & 0xff) << 16 | (data[1] & 0xff) << 8 | (data[0] & 0xff);
        }
    }
}
