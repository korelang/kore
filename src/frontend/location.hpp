#ifndef KORE_LOCATION_HPP
#define KORE_LOCATION_HPP

#include <cstddef>
#include <ostream>

namespace kore {
    /// A location of a token in an input source
    class Location {
        private:
            int _lnum;
            int _start_col;
            int _end_col;

        public:
            Location();
            Location(const Location& location);
            Location(const Location& start, const Location& end);
            Location(int lnum, int start_col, int end_col);
            virtual ~Location();

            int lnum() const noexcept;
            int start() const noexcept;
            int end() const noexcept;
            std::string format_columns() const;
            std::string colon_format() const;

            static const Location unknown;
    };

    std::ostream& operator<<(std::ostream& os, const Location& location);
}

#endif // KORE_LOCATION_HPP
