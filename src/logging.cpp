#include <cstdio>
#include <iostream>

#include "logging.hpp"

void output(
    const std::string& level,
    const std::string& group,
    const std::string& color,
    bool newline,
    const char* const format,
    va_list args
) {
    std::cerr << color << "[" << level;

    if (!group.empty()) {
        std::cerr << ":" << group;
    }

    std::cerr << "] " << "\x1b[0m";

    std::vfprintf(stderr, format, args);

    if (newline) {
        std::cerr << std::endl;
    }
}

void debug(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("debug", "", "\x1b[36;1;96m", true, format, args);
    va_end(args);
}

void debug_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("debug", group, "\x1b[36;1;96m", true, format, args);
    va_end(args);
}

void info(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("info", "", "\x1b[37;1;97m", true, format, args);
    va_end(args);
}

void info_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("info", group, "\x1b[37;1;97m", true, format, args);
    va_end(args);
}

void warn(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("warn", "", "\x1b[33;1;93m", true, format, args);
    va_end(args);
}

void warn_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("warn", group, "\x1b[33;1;93m", true, format, args);
    va_end(args);
}

void error(const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("error", "", "\x1b[31;1;91m", true, format, args);
    va_end(args);
}

void error_group(const std::string& group, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("error", group, "\x1b[31;1;91m", true, format, args);
    va_end(args);
}

void parser_error(const Location& location, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    output("error", "parser", "\x1b[31;1;91m", false, format, args);
    va_end(args);

    std::cerr << " (" << location << ")" << std::endl;
}
