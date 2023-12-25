#include "logging/color.hpp"
#include "logging/color_spec.hpp"

#include <ios>
#include <iostream>

namespace kore {
    inline int get_color_flag_index() {
        static int idx = std::ios_base::xalloc();

        return idx;
    }

    std::ostream& colors(std::ostream& os, bool enabled) {
        os.iword(get_color_flag_index()) = static_cast<int>(enabled);

        return os;
    }

    Color::Color() : Color(AnsiColorCode::Black) {}

    Color::Color(AnsiColorCode ansi_code) : _ansi_code(ansi_code) {}

    Color::~Color() {}

    bool Color::is_background() const noexcept {
        return _is_background;
    }

    const Color Color::None(AnsiColorCode::Reset);

    const Color Color::Black(AnsiColorCode::Black);
    const Color Color::Red(AnsiColorCode::Red);
    const Color Color::Green(AnsiColorCode::Green);
    const Color Color::Yellow(AnsiColorCode::Yellow);
    const Color Color::Blue(AnsiColorCode::Blue);
    const Color Color::Magenta(AnsiColorCode::Magenta);
    const Color Color::Cyan(AnsiColorCode::Cyan);
    const Color Color::White(AnsiColorCode::White);

    const Color Color::Gray(AnsiColorCode::Gray);
    const Color Color::Grey(AnsiColorCode::Grey);
    const Color Color::BrightRed(AnsiColorCode::BrightRed);
    const Color Color::BrightGreen(AnsiColorCode::BrightGreen);
    const Color Color::BrightYellow(AnsiColorCode::BrightYellow);
    const Color Color::BrightBlue(AnsiColorCode::BrightBlue);
    const Color Color::BrightMagenta(AnsiColorCode::BrightMagenta);
    const Color Color::BrightCyan(AnsiColorCode::BrightCyan);
    const Color Color::BrightWhite(AnsiColorCode::BrightWhite);

    std::ostream& operator<<(std::ostream& os, const Color& color) {
        int code = static_cast<int>(color._ansi_code);

        if (code != 0 && color.is_background()) {
            code += 10;
        }

        return os << "\x1b[" << code << "m";
    }

    std::ostream& operator<<(std::ostream& os, const ColorSpec& color_spec) {
        return os << color_spec.attribute << color_spec.color;
    }
}
