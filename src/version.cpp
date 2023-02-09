#include <iostream>

#include "version.hpp"

namespace kore {
    std::ostream& operator<<(std::ostream& out, const Version& version) {
        out << version.major << "."
            << version.minor << "."
            << version.patch;

        return out;
    }

    void print_version(const std::string& name, kore::Version version, bool version_only) {
        if (version_only) {
            std::cout << version << std::endl;
        } else {
            std::cout << name.c_str() << " v" << version << std::endl;
        }
    }
}
