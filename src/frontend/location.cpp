#include <sstream>

#include "location.hpp"

namespace kore {
    Location::Location() : Location(-1, -1, -1) {
    }

    Location::Location(const Location& location)
        : _lnum(location.lnum()),
        _start_col(location.start()),
        _end_col(location.end()) {
    }

    // TODO: Support multi-line locations (due to e.g. arbitrary line breaks)
    Location::Location(const Location& start, const Location& end)
        : _lnum(start.lnum()),
        _start_col(start.start()),
        _end_col(end.end()) {
    }

    Location::Location(int lnum, int start_col, int end_col)
        : _lnum(lnum), _start_col(start_col), _end_col(end_col) {
    }

    Location::~Location() {
    }

    int Location::lnum() const noexcept {
        return this->_lnum;
    }

    int Location::start() const noexcept {
        return this->_start_col;
    }

    int Location::end() const noexcept {
        return this->_end_col;
    }

    std::string Location::format_columns() const {
        std::ostringstream oss;
        oss << (start() + 1);

        if (start() < end()) {
            oss << end();
        }

        return oss.str();
    }

    std::string Location::colon_format() const {
        std::ostringstream oss;
        oss << lnum() << ':' << (start() + 1);

        if (start() < end()) {
            oss << '-' << (end() + 1);
        }

        return oss.str();
    }

    const Location Location::unknown = Location();

    std::ostream& operator<<(std::ostream& os, const Location& location) {
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
