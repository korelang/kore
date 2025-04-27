#ifndef KORE_SOURCE_LOCATION_HPP
#define KORE_SOURCE_LOCATION_HPP

#include <cstddef>
#include <ostream>

namespace kore {
    /// A location of a token in an input source
    class SourceLocation {
        using Pos = int;

        public:
            SourceLocation();
            SourceLocation( const SourceLocation& location);
            SourceLocation(const SourceLocation& start, const SourceLocation& end);
            SourceLocation(Pos line, Pos start_col, Pos end_col);
            SourceLocation(Pos start_line, Pos end_line, Pos start_col, Pos end_col);
            virtual ~SourceLocation();

            Pos start_line() const noexcept;
            Pos end_line() const noexcept;
            Pos start_col() const noexcept;
            Pos end_col() const noexcept;

            bool is_single_col() const noexcept;
            bool is_multiline() const noexcept;
            bool is_unknown() const noexcept;

            std::string format_lines() const;
            std::string format_columns() const;
            std::string format() const;
            std::string format_verbose() const;

            void merge(const SourceLocation& location);

            static const SourceLocation unknown;

        private:
            Pos _start_line;
            Pos _end_line;
            Pos _start_col;
            Pos _end_col;

            static char _RANGE_SEP;
            static char _SEP;
    };

    std::ostream& operator<<(std::ostream& os, const SourceLocation& location);
}

#endif // KORE_SOURCE_LOCATION_HPP
