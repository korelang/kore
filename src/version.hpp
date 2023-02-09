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

    void print_version(const std::string& name, kore::Version version, bool version_only);
}

#endif // KORE_VERSION_HPP
