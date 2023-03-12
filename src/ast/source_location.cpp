#include <sstream>

#include "source_location.hpp"

namespace kore {
    SourceLocation::SourceLocation() : SourceLocation(-1, -1, -1) {
    }

    SourceLocation::SourceLocation(const SourceLocation& location)
        : _lnum(location.lnum()),
        _start_col(location.start()),
        _end_col(location.end()) {
    }

    // TODO: Support multi-line locations (due to e.g. arbitrary line breaks)
    SourceLocation::SourceLocation(const SourceLocation& start, const SourceLocation& end)
        : _lnum(start.lnum()),
        _start_col(start.start()),
        _end_col(end.end()) {
    }

    SourceLocation::SourceLocation(int lnum, int start_col, int end_col)
        : _lnum(lnum), _start_col(start_col), _end_col(end_col) {
    }

    SourceLocation::~SourceLocation() {
    }

    int SourceLocation::lnum() const noexcept {
        return this->_lnum;
    }

    int SourceLocation::start() const noexcept {
        return this->_start_col;
    }

    int SourceLocation::end() const noexcept {
        return this->_end_col;
    }

    bool SourceLocation::is_single_pos() const noexcept {
        return _start_col == _end_col;
    }

    bool SourceLocation::is_unknown() const noexcept {
        return _lnum == -1 && _start_col == -1 && _end_col == -1;
    }

    std::string SourceLocation::format_columns() const {
        std::ostringstream oss;
        oss << (start() + 1);

        if (start() < end()) {
            oss << end();
        }

        return oss.str();
    }

    std::string SourceLocation::colon_format() const {
        std::ostringstream oss;
        oss << lnum() << ':' << (start() + 1);

        if (start() < end()) {
            oss << '-' << (end() + 1);
        }

        return oss.str();
    }

    const SourceLocation SourceLocation::unknown = SourceLocation();

    std::ostream& operator<<(std::ostream& os, const SourceLocation& location) {
        os << "line " << location.lnum();

        if (location.start() != location.end()) {
            os << ", columns "
            << (location.start() + 1)
            << "-"
            << (location.end() + 1);
        } else {
            os << ", column " << (location.start() + 1);
        }

        return os;
    }
}
