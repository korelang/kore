#ifndef KORE_LOGGING_HPP
#define KORE_LOGGING_HPP

#include "location.hpp"
#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"

namespace kore {
    void success(const char* const format, ...);

    void success(int verbosity, int options_verbosity, const char* const format, ...);

    void success_group(const std::string& group, const char* const format, ...);

    void success_group(int verbosity, int options_verbosity, const std::string& group, const char* const format, ...);

    void debug(const char* const format, ...);

    void debug_group(const std::string& group, const char* const format, ...);

    void info(const char* const format, ...);

    void info_group(const std::string& group, const char* const format, ...);

    void warn(const char* const format, ...);

    void warn_group(const std::string& group, const char* const format, ...);

    void error(const char* const format, ...);

    void error_indent(const char* const format, ...);

    void error_group(const std::string& group, const char* const format, ...);

    void section(const std::string& group, Color color, ColorAttribute attributes, const char* const format, ...);

    void parser_error(const Location& location, const char* const format, ...);
}

#endif // KORE_LOGGING_HPP
