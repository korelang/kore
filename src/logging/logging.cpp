#include <cstdio>
#include <iostream>

#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"
#include "logging/logging.hpp"

// TODO: Distinguish between stderr and stdout
void output(
    const std::string& level,
    const std::string& group,
    const Color& color,
    const ColorAttribute& attribute,
    bool newline,
    const char* const format,
    va_list args
) {
    std::cerr << color << attribute << "[" << level;

    if (!group.empty()) {
        std::cerr << ":" << group;
    }

    std::cerr << "] " << ColorAttribute::Reset;

    std::vfprintf(stderr, format, args);

    if (newline) {
        std::cerr << std::endl;
    }
}

void success(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("success", "", Color::Green, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void success_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("success", group, Color::Green, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void debug(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("debug", "", Color::Cyan, ColorAttribute::None, true, format, args);
    va_end(args);
}

void debug_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("debug", group, Color::Cyan, ColorAttribute::None, true, format, args);
    va_end(args);
}

void info(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("info", "", Color::White, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void info_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("info", group, Color::White, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void warn(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("warn", "", Color::Yellow, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void warn_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("warn", group, Color::Yellow, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void error(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("error", "", Color::Red, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void error_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("error", group, Color::Red, ColorAttribute::Bold, true, format, args);
    va_end(args);
}

void parser_error(const Location& location, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("error", "parser", Color::Red, ColorAttribute::Bold, false, format, args);
    va_end(args);

    std::cerr << " (" << location << ")" << std::endl;
}
