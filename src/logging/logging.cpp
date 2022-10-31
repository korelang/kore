#include <cstdio>
#include <iostream>

#include "logging/logging.hpp"

namespace kore {
    // TODO: Distinguish between stderr and stdout
    void output(
        int indent,
        const std::string& level,
        const std::string& group,
        const Color& color,
        const ColorAttribute& attribute,
        bool newline,
        const char* const format,
        va_list args
    ) {
        std::cerr << color << attribute;

        if (indent > 0) {
            std::cerr << "    ";
        }

        std::cerr << "[" << level;

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
        output(0, "success", "", Color::Green, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void success(int verbosity, int options_verbosity, const char* const format, ...) {
        if (options_verbosity < verbosity) {
            return;
        }

        va_list args;
        va_start(args, format);
        output(0, "success", "", Color::Green, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void success_group(const std::string& group, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "success", group, Color::Green, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void success_group(int verbosity, int options_verbosity, const std::string& group, const char* const format, ...) {
        if (options_verbosity < verbosity) {
            return;
        }

        va_list args;
        va_start(args, format);
        output(0, "success", group, Color::Green, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void debug(const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "debug", "", Color::Cyan, ColorAttribute::None, true, format, args);
        va_end(args);
    }

    void debug_group(const std::string& group, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "debug", group, Color::Cyan, ColorAttribute::None, true, format, args);
        va_end(args);
    }

    void info(const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "info", "", Color::White, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void info_group(const std::string& group, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "info", group, Color::White, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void warn(const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "warn", "", Color::Yellow, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void warn_group(const std::string& group, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "warn", group, Color::Yellow, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void error(const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "error", "", Color::Red, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void error_indent(const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(1, "error", "", Color::Red, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void error_group(const std::string& group, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "error", group, Color::Red, ColorAttribute::Bold, true, format, args);
        va_end(args);
    }

    void section(const std::string& group, Color color, ColorAttribute attributes, int indent, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(indent, group, "", color, attributes, true, format, args);
        va_end(args);
    }

    void parser_error(const Location& location, const char* const format, ...) {
        va_list args;
        va_start(args, format);
        output(0, "error", "parser", Color::Red, ColorAttribute::Bold, false, format, args);
        va_end(args);

        std::cerr << " (" << location << ")" << std::endl;
    }
}
