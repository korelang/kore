#ifndef KORE_LOGGING_HPP
#define KORE_LOGGING_HPP

#include "diagnostics/diagnostic.hpp"
#include "errors/error.hpp"
#include "logging/color_attributes.hpp"
#include "logging/color.hpp"
#include "logging/color_spec.hpp"
#include "ast/source_location.hpp"

namespace kore {
    enum class Level {
        Debug = 0,
        Info,
        Success,
        Warn,
        Error,
        Critical
    };

    /* void log(const char* const format, ...); */
    /* void log(int verbosity, int options_verbosity, const char* const format, ...); */
    /* void log_group(const std::string& group, const char* const format, ...); */
    /* void log_group(const std::string& group, int verbosity, int options_verbosity, const char* const format, ...); */

    void success(const char* const format, ...);

    void success(int verbosity, int options_verbosity, const char* const format, ...);

    void success_group(const std::string& group, const char* const format, ...);

    void success_group(int verbosity, int options_verbosity, const std::string& group, const char* const format, ...);

    void debug(const char* const format, ...);

    void debug_group(const std::string& group, const char* const format, ...);

    void log_group(const std::string& group, const std::ostringstream& oss, const std::string& level);

    void info(const char* const format, ...);

    void info_group(const std::string& group, const char* const format, ...);

    void warn(const char* const format, ...);

    void warn_group(const std::string& group, const char* const format, ...);

    void error(const char* const format, ...);

    void error_indent(const char* const format, ...);

    void error_group(const std::string& group, const char* const format, ...);

    void critical_group(const std::string& group, const char* const format, ...);

    void section(
        const std::string& group,
        Color color,
        ColorAttribute attributes,
        int indent,
        const char* const format,
        ...
    );

    void section(
        const std::string& group,
        const ColorSpec color_spec,
        int indent,
        const char* const format,
        ...
    );

    void section(const Diagnostic& diagnostic, int indent);

    void section_error(const std::string& group, errors::Error error, int indent);

    void parser_error(const SourceLocation& location, const char* const format, ...);
}

#endif // KORE_LOGGING_HPP
