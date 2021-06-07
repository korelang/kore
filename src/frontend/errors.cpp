#include <exception>
#include <iomanip>
#include <sstream>

#include "errors.hpp"

std::string format_error_at_line(
    const std::string& line,
    std::size_t start_col,
    std::size_t end_col
) {
    std::ostringstream oss;

    const std::string indent = "|" + std::string(4, ' ');

    if (start_col == end_col) {
        oss << indent << line << std::endl
            << std::setw(end_col + 1) << "^";
    } else {
        oss << indent << line << std::endl
            << indent
            << std::setw(start_col + 1)
            << "^"
            << std::setfill('-')
            << std::setw(end_col - start_col)
            << "^";
    }

    return oss.str();
}

std::string format_error_at_line(const std::string& line, const Location& location) {
    return format_error_at_line(line, location.start(), location.end());
}

std::string format_error(
    const std::string& msg,
    const std::string& line,
    std::size_t lnum,
    std::size_t start_col,
    std::size_t end_col
) {
    std::ostringstream oss;

    if (start_col == end_col) {
        oss << msg << " (line " << lnum << ", column " << end_col << ")"
            << std::endl;
    } else {
        oss << msg << " (line " << lnum
            << ", columns " << (start_col + 1) << "-" << (end_col + 1) << ")"
            << std::endl;
    }

    format_error_at_line(line, start_col, end_col);

    return oss.str();
}

std::string format_error(
    const std::string& msg,
    const std::string& line,
    const Location& location
) {
    return format_error(msg, line, location.lnum(), location.start(), location.end());
}

void throw_error_for_line(
    const std::string& msg,
    const std::string& line,
    std::size_t lnum,
    std::size_t start_col,
    std::size_t end_col
) {
    throw std::runtime_error(format_error(msg, line, lnum, start_col, end_col));
}
