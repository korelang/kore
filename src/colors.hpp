#ifndef KORE_COLORS_HPP
#define KORE_COLORS_HPP

#include <ostream>

enum class AnsiColorCode {
    Reset = 0,
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    White = 37,

    Gray = 90,
    Grey = 90,
    BrightRed = 91,
    BrightGreen = 92,
    BrightYellow = 93,
    BrightBlue = 94,
    BrightMagenta = 95,
    BrightCyan = 96,
    BrightWhite = 97,
};

class Color final {
    friend std::ostream& operator<<(std::ostream& os, const Color& color);

    public:
        Color();
        Color(AnsiColorCode ansi_code);
        virtual ~Color();

        bool is_background() const noexcept;

        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Yellow;
        static const Color Blue;
        static const Color Magenta;
        static const Color Cyan;
        static const Color White;

        static const Color Gray;
        static const Color Grey;
        static const Color BrightRed;
        static const Color BrightGreen;
        static const Color BrightYellow;
        static const Color BrightBlue;
        static const Color BrightMagenta;
        static const Color BrightCyan;
        static const Color BrightWhite;

    private:
        AnsiColorCode _ansi_code;
        bool _is_background;
};

std::ostream& operator<<(std::ostream& os, const Color& color);

#endif // KORE_COLORS_HPP
