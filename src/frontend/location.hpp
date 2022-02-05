#ifndef KORE_LOCATION_HPP
#define KORE_LOCATION_HPP

#include <cstddef>
#include <ostream>

namespace kore {
    /// A location of a token in an input source
    class Location {
        private:
            std::size_t _lnum;
            std::size_t _start_col;
            std::size_t _end_col;

        public:
            Location();
            Location(const Location& location);
            Location(const Location& start, const Location& end);
            Location(std::size_t lnum, std::size_t start_col, std::size_t end_col);
            virtual ~Location();

            std::size_t lnum() const noexcept;
            std::size_t start() const noexcept;
            std::size_t end() const noexcept;
            std::string format_columns() const;

            static const Location unknown;
    };

    std::ostream& operator<<(std::ostream& os, const Location& location);
}

#endif // KORE_LOCATION_HPP
