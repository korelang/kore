#include "color_attributes.hpp"

ColorAttribute::ColorAttribute() : ColorAttribute(AnsiAttributeCode::Reset) {}

ColorAttribute::ColorAttribute(AnsiAttributeCode attribute)
    : _attribute_code(attribute) {}

ColorAttribute::~ColorAttribute() {}

const ColorAttribute ColorAttribute::None(AnsiAttributeCode::None);
const ColorAttribute ColorAttribute::Reset(AnsiAttributeCode::Reset);
const ColorAttribute ColorAttribute::Bold(AnsiAttributeCode::Bold);
const ColorAttribute ColorAttribute::Faint(AnsiAttributeCode::Faint);
const ColorAttribute ColorAttribute::Italic(AnsiAttributeCode::Italic);
const ColorAttribute ColorAttribute::Underline(AnsiAttributeCode::Underline);
const ColorAttribute ColorAttribute::SlowBlink(AnsiAttributeCode::SlowBlink);
const ColorAttribute ColorAttribute::FastBlink(AnsiAttributeCode::FastBlink);
const ColorAttribute ColorAttribute::Reverse(AnsiAttributeCode::Reverse);

std::ostream& operator<<(std::ostream& os, const ColorAttribute& attribute) {
    if (attribute._attribute_code != AnsiAttributeCode::None) {
        return os << "\x1b[" << static_cast<int>(attribute._attribute_code) << "m";
    }

    return os;
}
