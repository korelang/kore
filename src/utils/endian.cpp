#include <array>
#include <istream>

#include "endian.hpp"

namespace kore {
    bool is_big_endian() {
        const std::uint32_t one = 1;

        return *(std::uint8_t*)&one != 1;
    }

    // TODO: Template these functions
    std::uint16_t read_be16(std::istream& is) {
        std::array<char, 2> data;
        is.read(data.data(), 2);

        if (is_big_endian()) {
            return (data[1] & 0xff) << 8 | (data[0] & 0xff);
        } else {
            return (data[0] & 0xff) << 8 | (data[1] & 0xff);
        }
    }

    std::uint32_t read_be32(std::istream& is) {
        std::array<char, 4> data;
        is.read(data.data(), 4);

        if (is_big_endian()) {
            return
                (data[3] & 0xff) << 24 |
                (data[2] & 0xff) << 16 |
                (data[1] & 0xff) << 8 |
                (data[0] & 0xff);
        } else {
            return
                (data[0] & 0xff) << 24 |
                (data[1] & 0xff) << 16 |
                (data[2] & 0xff) << 8 |
                (data[3] & 0xff);
        }
    }

    std::uint32_t read_le32(std::istream& is) {
        std::array<char, 4> data;
        is.read(data.data(), 4);

        if (is_big_endian()) {
            return
                (data[0] & 0xff) << 24 |
                (data[1] & 0xff) << 16 |
                (data[2] & 0xff) << 8 |
                (data[3] & 0xff);
        } else {
            return
                (data[3] & 0xff) << 24 |
                (data[2] & 0xff) << 16 |
                (data[1] & 0xff) << 8 |
                (data[0] & 0xff);
        }
    }

    /* std::uint64_t read_be64(std::istream& is) { */
    /*     union endian_data { */
    /*         std::array<char, 8> data; */
    /*         std::uint64_t value; */
    /*     } */

    /*     is.read(data.data(), 8); */

    /*     if (is_big_endian()) { */
    /*         return */
    /*             (data[0] & 0xff) << 56 */
    /*             | (data[1] & 0xff) << 48 */
    /*             | (data[2] & 0xff) << 40 */
    /*             | (data[3] & 0xff) << 32 */
    /*             | (data[4] & 0xff) << 24 */
    /*             | (data[5] & 0xff) << 16 */
    /*             | (data[6] & 0xff) << 8 */
    /*             | (data[7] & 0xff); */
    /*     } else { */
    /*         return */
    /*             (data[7] & 0xff) << 56 */
    /*             | (data[6] & 0xff) << 48 */
    /*             | (data[5] & 0xff) << 40 */
    /*             | (data[4] & 0xff) << 32 */
    /*             | (data[3] & 0xff) << 24 */
    /*             | (data[2] & 0xff) << 16 */
    /*             | (data[1] & 0xff) << 8 */
    /*             | (data[0] & 0xff); */
    /*     } */
    /* } */

    void write_be16(std::uint16_t value, std::ostream& os) {
        if (!is_big_endian()) {
            value = value << 8 | value >> 8;
        }

        os.write((char*)&value, 2);
    }

    void write_be32(std::uint32_t value, std::ostream& os) {
        if (!is_big_endian()) {
            value =
                (value << 24) |
                ((value & 0xff00) << 8) |
                ((value >> 8) & 0xff00) |
                (value >> 24);
        }

        os.write((char*)&value, 4);
    }

    std::uint32_t read_le32(Buffer& buffer) {
        if (is_big_endian()) {
            return
                (buffer[0] & 0xff) << 24 |
                (buffer[1] & 0xff) << 16 |
                (buffer[2] & 0xff) << 8 |
                (buffer[3] & 0xff);
        } else {
            return
                (buffer[3] & 0xff) << 24 |
                (buffer[2] & 0xff) << 16 |
                (buffer[1] & 0xff) << 8 |
                (buffer[0] & 0xff);
        }
    }

    void write_le32(std::uint32_t value, Buffer& buffer) {
        if (is_big_endian()) {
            buffer.insert(
                buffer.end(),
                {
                    static_cast<std::uint8_t>(value >> 24),
                    static_cast<std::uint8_t>((value >> 16) & 0xff),
                    static_cast<std::uint8_t>((value >> 8) & 0xff),
                    static_cast<std::uint8_t>(value & 0xff)
                }
            );
        } else {
            buffer.insert(
                buffer.end(),
                {
                    static_cast<std::uint8_t>(value & 0xff),
                    static_cast<std::uint8_t>((value >> 8) & 0xff),
                    static_cast<std::uint8_t>((value >> 16) & 0xff),
                    static_cast<std::uint8_t>(value >> 24)
                }
            );
        }
    }
}
