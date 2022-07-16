#include "version.hpp"

namespace kore {
    std::ostream& operator<<(std::ostream& out, const Version& version) {
        out << version.major << "."
            << version.minor << "."
            << version.patch;

        return out;
    }
}
