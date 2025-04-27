#include <ostream>

#include "source_location.hpp"

namespace kore {
    SourceLocation::SourceLocation() : SourceLocation(-1, -1, -1, -1) {
    }

    SourceLocation::SourceLocation(const SourceLocation& location)
        : kore::SourceLocation(
            location.start_line(),
            location.end_line(),
            location.start_col(),
            location.end_col()
        )
    {}

    SourceLocation::SourceLocation(
        const SourceLocation& start,
        const SourceLocation& end
    ) : SourceLocation(start) {
        merge(end);
    }

    SourceLocation::SourceLocation(Pos line, Pos start_col, Pos end_col)
        : SourceLocation(line, line, start_col, end_col)
    {}

    SourceLocation::SourceLocation(Pos start_line, Pos end_line, Pos start_col, Pos end_col)
        : _start_line(start_line),
          _end_line(end_line),
          _start_col(start_col),
          _end_col(end_col)
    {}

    SourceLocation::~SourceLocation() {}

    SourceLocation::Pos SourceLocation::start_line() const noexcept {
        return _start_line;
    }

    SourceLocation::Pos SourceLocation::end_line() const noexcept {
        return _end_line;
    }

    SourceLocation::Pos SourceLocation::start_col() const noexcept {
        return _start_col;
    }

    SourceLocation::Pos SourceLocation::end_col() const noexcept {
        return _end_col;
    }

    bool SourceLocation::is_single_col() const noexcept {
        return _start_col == _end_col;
    }

    bool SourceLocation::is_multiline() const noexcept {
        return _start_line != _end_line;
    }

    bool SourceLocation::is_unknown() const noexcept {
        return _start_line == -1 || _end_line == -1 || _start_col == -1 || _end_col == -1;
    }

    std::string SourceLocation::format_lines() const {
        if (_end_line > _start_line) {
            return std::to_string(_start_line) + _RANGE_SEP + std::to_string(_end_line);
        }

        return std::to_string(_start_line);
    }

    std::string SourceLocation::format_columns() const {
        if (_end_col > _start_col) {
            return std::to_string(_start_col + 1) + _RANGE_SEP + std::to_string(_end_col + 1);
        }

        return std::to_string(_start_col + 1);
    }

    std::string SourceLocation::format() const {
        return format_lines() + _SEP + format_columns();
    }

    std::string SourceLocation::format_verbose() const {
        return "";
    }

    void SourceLocation::merge(const SourceLocation& location) {
        _start_line = std::min(_start_line, location.start_line());
        _end_line = std::max(_end_line, location.end_line());
        _start_col = std::min(_start_col, location.start_col());
        _end_col = std::max(_end_col, location.end_col());
    }

    const SourceLocation SourceLocation::unknown = SourceLocation();

    std::ostream& operator<<(std::ostream& os, const SourceLocation& location) {
        if (location.is_multiline()) {
            os << "lines " << location.start_line() << "-" << location.end_line();
        } else {
            os << "line " << location.start_line();
        }

        if (location.is_single_col()) {
            os << ", column " << (location.start_col() + 1);
        } else {
            os << ", columns " << (location.start_col() + 1) << "-" << (location.end_col() + 1);
        }

        return os;
    }

    char SourceLocation::_RANGE_SEP = '-';
    char SourceLocation::_SEP = ':';
}
