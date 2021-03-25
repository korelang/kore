#ifndef KORE_LOCATION_HPP
#define KORE_LOCATION_HPP

#include <cstddef>
#include <ostream>

/// A location of a token in an input source
class Location {
    private:
        std::size_t _lnum;
        std::size_t _start_col;
        std::size_t _end_col;

        Location();

    public:
        Location(std::size_t lnum, std::size_t start_col, std::size_t end_col);
        virtual ~Location();

        std::size_t lnum() const noexcept;
        std::size_t start() const noexcept;
        std::size_t end() const noexcept;

        static const Location unknown;
};

std::ostream& operator<<(std::ostream& os, const Location& location);

#endif // KORE_LOCATION_HPP
