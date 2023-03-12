#ifndef KORE_SOURCE_LOCATION_HPP
#define KORE_SOURCE_LOCATION_HPP

#include <cstddef>
#include <ostream>

namespace kore {
    /// A location of a token in an input source
    class SourceLocation {
        private:
            int _lnum;
            int _start_col;
            int _end_col;

        public:
            SourceLocation();
            SourceLocation(const SourceLocation& location);
            SourceLocation(const SourceLocation& start, const SourceLocation& end);
            SourceLocation(int lnum, int start_col, int end_col);
            virtual ~SourceLocation();

            int lnum() const noexcept;
            int start() const noexcept;
            int end() const noexcept;
            bool is_single_pos() const noexcept;
            bool is_unknown() const noexcept;
            std::string format_columns() const;
            std::string colon_format() const;

            static const SourceLocation unknown;
    };

    std::ostream& operator<<(std::ostream& os, const SourceLocation& location);
}

#endif // KORE_SOURCE_LOCATION_HPP
