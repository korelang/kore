#ifndef KORE_VERSION_HPP
#define KORE_VERSION_HPP

#include <ostream>

namespace kore {
    struct Version {
        int major;
        int minor;
        int patch;
    };

    std::ostream& operator<<(std::ostream& out, const Version& version);
}

#endif // KORE_VERSION_HPP
