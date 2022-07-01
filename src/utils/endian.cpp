#include "endian.hpp"

namespace kore {
    bool is_big_endian() {
        const int one = 1;

        return *(char*)(&one) != 1;
    }
}
