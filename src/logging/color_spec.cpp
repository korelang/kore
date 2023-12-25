#include "color_spec.hpp"

namespace kore {
    std::ostream& operator<<(std::ostream& os, const ColorSpec& color_spec) {
        return os << color_spec.attribute << color_spec.color;
    }
}
