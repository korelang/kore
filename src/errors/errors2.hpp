#ifndef KORE_ERRORS_HPP
#define KORE_ERRORS_HPP

#include <string>

#include "errors/error.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "source_location.hpp"

namespace kore {
    namespace errors {
        namespace typing {
            Error cannot_assign(const Type* lhs, const Type* rhs, const SourceLocation& location);
            Error incompatible_binop(const Type* left, const Type* right, BinOp op, const SourceLocation& location);
            Error binop_numeric_operands(const Type* left, const Type* right, BinOp op, const SourceLocation& location);
            Error variables_shadows(const Identifier* identifier);
        }
    }

    std::string format_locations(const SourceLocation& start, const SourceLocation& end);

    /* void output_error( */
    /*     const errors::Error& error, */
    /*     const std::string& source_name */
    /* ); */

    std::string format_error_at_line(
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    );

    std::string format_error_at_line(const std::string& line, const SourceLocation& location);

    std::string format_error(
        const std::string& msg,
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    );

    std::string format_error(
        const std::string& msg,
        const std::string& line,
        const SourceLocation& location
    );

    void throw_error_for_line(
        const std::string& msg,
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    );
}

#endif // KORE_ERRORS_HPP
