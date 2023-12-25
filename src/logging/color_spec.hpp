#ifndef KORE_COLOR_SPEC_HPP
#define KORE_COLOR_SPEC_HPP

#include "logging/color.hpp"
#include "logging/color_attributes.hpp"

#include <ostream>

namespace kore {
    class Color;

    struct ColorSpec {
        const Color color = Color::None;
        const ColorAttribute attribute = ColorAttribute::None;
    };

    std::ostream& operator<<(std::ostream& os, const ColorSpec& color_spec);
}

#endif // KORE_COLOR_SPEC_HPP
