#ifndef KORE_ENDIAN_HPP
#define KORE_ENDIAN_HPP

#include <istream>
#include <ostream>
#include <vector>

namespace kore {
    using Buffer = std::vector<std::uint8_t>;

    bool is_big_endian();

    template<unsigned int N>
    void read_le(std::istream& is) {
        static_assert(N % 8 == 0, "N must be divisble by 8");
        static_assert(N >= 16 && N <= 64, "N must be 16, 32, or 64");

        constexpr decltype(N) byte_count = N / 8;
        std::array<char, byte_count> data;
        auto bytes_read = is.read(data.data(), N).gcount();

        if (bytes_read != byte_count) {
            if (is.eof()) {
                throw new std::runtime_error("Prematurely reached end of stream");
            } else {
                throw new std::runtime_error("Failed to read from stream");
            }
        }

        // 1 2 3 4
        // Saved on LE: 4 3 2 1
        // Saved on BE: 1 2 3 4 => 4 3 2 1

        // Saved on LE: 1 2 3 4 => 4 3 2 1 => 1 2 3 4
        // Saved on BE: 1 2 3 4 => 4 3 2 1

        decltype(N) result = 0;

        if (!is_big_endian()) {
            return data.data();
        } else {
            for (int i = 0; i < N / 8; ++i) {

            }
        }
    }

    template<typename T>
    void write_big_endian(T value, std::ostream& os) {
        constexpr auto size = sizeof(T);

        if (!is_big_endian()) {
            char data[size];
            char* ptr = (char*)&value;

            for (std::size_t i = 0; i < size; ++i) {
                data[i] = ptr[size - i - 1];
            }

            os.write(data, size);
        } else {
            os.write((char*)&value, size);
        }
    }

    /* template<int N, typename T> */
    /* T read_be16(std::istream& is) { */
    /*     std::array<char, N> data; */
    /*     is.read(data.data(), N); */

    /*     T value = 0; */

    /*     for(int i = 0; i < int(sizeof); ++i) { */
    /*         h <<= 8; */
    /*         h |= static_cast<uint8_t>(*it++); */
    /*     } */
    /*     return h; */
    /*     if (is_big_endian()) { */
    /*         return (data[0] & 0xff) << 8 | (data[1] & 0xff); */
    /*     } else { */
    /*         return (data[1] & 0xff) << 8 | (data[0] & 0xff); */
    /*     } */
    /* } */

    /* template<int N, typename T> */
    /* void write_be(T value, std::ostream& os) { */
    /*     constexpr auto size = sizeof(T); */
    /*     static_assert(size < N, "Oh noes..."); */

    /*     if (!is_big_endian()) { */
    /*         char data[size]; */
    /*         char* ptr = (char*)&value; */

    /*         for (std::size_t i = 0; i < size; ++i) { */
    /*             data[i] = ptr[size - i - 1]; */
    /*         } */

    /*         os.write(data, size); */
    /*     } else { */
    /*         os.write((char*)&value, size); */
    /*     } */
    /* } */

    // TODO: Make these functions more generic!

    // Stream-based endian functions
    std::uint16_t read_be16(std::istream& is);
    std::uint32_t read_be32(std::istream& is);
    std::uint32_t read_le32(std::istream& is);
    /* std::uint64_t read_be64(std::istream& is); */
    void write_be16(std::uint16_t value, std::ostream& os);
    void write_be32(std::uint32_t value, std::ostream& os);
    void write_le32(std::uint32_t value, std::ostream& os);
    /* void write_be64(std::uint32_t value, std::ostream& os); */

    // Buffer-based endian functions
    std::uint32_t read_le32(Buffer& buffer);
    void write_le32(std::uint32_t value, Buffer& buffer);
    void write_be32(std::uint32_t value, Buffer& buffer);

    void read_be32_bytes(std::istream& is, std::size_t count, std::vector<std::uint32_t>& buffer);
    void write_le32_bytes(std::vector<std::uint32_t>& buffer);
}

#endif // KORE_ENDIAN_HPP
