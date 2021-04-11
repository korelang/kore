#ifndef KORE_COLOR_ATTRIBUTES_HPP
#define KORE_COLOR_ATTRIBUTES_HPP

#include <ostream>

enum class AnsiAttributeCode {
    None = -1,
    Reset = 0,
    Bold = 1,
    Faint = 2,
    Italic = 3,
    Underline = 4,
    SlowBlink = 5,
    FastBlink = 6,
    Reverse = 7,
};

class ColorAttribute final {
    friend std::ostream& operator<<(std::ostream& os, const ColorAttribute& attribute);

    public:
        ColorAttribute();
        ColorAttribute(AnsiAttributeCode ansi_code);
        virtual ~ColorAttribute();

        static const ColorAttribute None;
        static const ColorAttribute Reset;
        static const ColorAttribute Bold;
        static const ColorAttribute Faint;
        static const ColorAttribute Italic;
        static const ColorAttribute Underline;
        static const ColorAttribute SlowBlink;
        static const ColorAttribute FastBlink;
        static const ColorAttribute Reverse;

    private:
        AnsiAttributeCode _attribute_code;
};

std::ostream& operator<<(std::ostream& os, const ColorAttribute& attribute);

#endif // KORE_COLOR_ATTRIBUTES_HPP
