#ifndef KORE_ERRORS_HPP
#define KORE_ERRORS_HPP

#include <string>

#include "errors/error.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "location.hpp"

namespace kore {
    namespace errors {
        namespace typing {
            Error cannot_assign(const Type* lhs, const Type* rhs, const Location& location);
            Error incompatible_binop(const Type* left, const Type* right, BinOp op, const Location& location);
            Error binop_numeric_operands(const Type* left, const Type* right, BinOp op, const Location& location);
            Error variables_shadows(const Identifier* identifier, const Location& location);
            Error undefined_variable(const Identifier* identifier);
        }
    }

    std::string format_locations(const Location& start, const Location& end);

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

    std::string format_error_at_line(const std::string& line, const Location& location);

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
        const Location& location
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
