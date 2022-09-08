#ifndef KORE_ENDIAN_HPP
#define KORE_ENDIAN_HPP

#include <ostream>

namespace kore {
    bool is_big_endian();

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

    std::uint16_t read_be16(std::istream& is);
    std::uint32_t read_be32(std::istream& is);
}

#endif // KORE_ENDIAN_HPP
