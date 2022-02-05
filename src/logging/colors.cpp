#include <iostream>
#include "colors.hpp"

namespace kore {
    Color::Color() : Color(AnsiColorCode::Black) {}

    Color::Color(AnsiColorCode ansi_code) : _ansi_code(ansi_code) {}

    Color::~Color() {}

    bool Color::is_background() const noexcept {
        return _is_background;
    }

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
        int final_code = static_cast<int>(color._ansi_code);

        if (final_code != 0 && color.is_background()) {
            final_code += 10;
        }

        return os << "\x1b[" << final_code << "m";
    }
}
