#include <sstream>

#include "location.hpp"

Location::Location() : Location(-1, -1, -1) {
}

Location::Location(std::size_t lnum, std::size_t start_col, std::size_t end_col)
    : _lnum(lnum), _start_col(start_col), _end_col(end_col) {
}

Location::~Location() {
}

std::size_t Location::lnum() const noexcept {
    return this->_lnum;
}

std::size_t Location::start() const noexcept {
    return this->_start_col;
}

std::size_t Location::end() const noexcept {
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
