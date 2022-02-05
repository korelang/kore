#ifndef KORE_ERRORS_HPP
#define KORE_ERRORS_HPP

#include <string>

#include "location.hpp"

namespace kore {
    std::string format_error_at_line(
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    );

    std::string format_error_at_line(const std::string& line, const Location& location);

    std::string format_error(
        const std::string& msg,
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    );

    std::string format_error(
        const std::string& msg,
        const std::string& line,
        const Location& location
    );

    void throw_error_for_line(
        const std::string& msg,
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    );
}

#endif // KORE_ERRORS_HPP
