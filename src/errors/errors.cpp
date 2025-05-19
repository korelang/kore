#include <iomanip>
#include <sstream>

#include "errors.hpp"
#include "ast/expressions/call.hpp"
#include "ast/expressions/identifier.hpp"
#include "types/function_type.hpp"
/* #include "logging.hpp" */

namespace kore {
    namespace errors {
        /* namespace scan { */
        /*     Error character_should_be_string(std::size_t col, std::size_t lnum) { */
        /*         auto message = "Character contains more than one character, should be a string?"; */

        /*         return Error(ErrorType::Scan, message, SourceLocation(lnum, col, col)); */
        /*     } */
        /* } */

        namespace typing {
            Error cannot_assign(const Type* lhs, const Type* rhs, const SourceLocation& location) {
                auto message = "cannot assign expression of type "
                    + lhs->name()
                    + " to variable of type "
                    + rhs->name()
                    + " without conversion";

                return Error(ErrorType::Typing, message, location);
            }

            Error incompatible_binop(const Type* left, const Type* right, BinOp op, const SourceLocation& location) {
                auto message = "cannot use binary operator '"
                    + binop_to_string(op) + "'"
                    + " with types "
                    + left->name()
                    + " and "
                    + right->name()
                    + " without conversion";

                return Error(ErrorType::Typing, message, location);
            }

            Error binop_operand(const Type* type, BinOp op, const std::string& which, const SourceLocation& location) {
                std::string types_string = operand_types_string(op);

                auto message = "binary operator '"
                    + binop_to_string(op)
                    + "' must have " + types_string + " operands but got "
                    + type->name() + " for " + which + " argument";

                return Error(ErrorType::Typing, message, location);
            }

            Error variable_shadows(const Identifier* identifier, const Identifier* shadowed, const SourceLocation& location, const SourceLocation& prev_location) {
                std::string message = "variable '" + identifier->name() + "' at (" + location.format() + ") shadows variable ";

                if (shadowed) {
                    message += "'" + shadowed->name() + "' (at " + prev_location.format() + ")";
                } else {
                    message += "in outer scope";
                }

                return Error(ErrorType::Typing, message, location);
            }

            Error redeclaration_constant_variable(const Identifier& identifier, const SourceLocation& location, const Identifier& prev_declared) {
                std::ostringstream oss;

                oss << "redeclaration of constant variable '" << identifier.name() << "', previously declared here: " << prev_declared.location();

                return Error(ErrorType::Typing, oss.str(), location);
            }

            Error cannot_declare_mutable_global(const Identifier& identifier, const SourceLocation& location) {
                auto message = "cannot declare global mutable variables ('" + identifier.name() + "')";

                return Error(ErrorType::Typing, message, location);
            }

            Error cannot_assign_global_variable(const Identifier* identifier, const Identifier* shadowed, const SourceLocation& location, const SourceLocation& prev_location) {
                std::string message = "assignment of '" + identifier->name() + "' at (" + location.format() + ") cannot assign to global variable '" + shadowed->name() + "' previously declared at (" + std::to_string(prev_location.start_line()) + ")";

                return Error(ErrorType::Typing, message, location);
            }

            Error undefined_variable(const Identifier& identifier) {
                return Error(ErrorType::Typing, "use of undefined variable " + identifier.name(), identifier.location());
            }

            Error unknown_call(const Call& call) {
                return Error(ErrorType::Typing, "unknown function called: " + call.name(), call.location());
            }

            Error incorrect_parameter_type(const Expression* expression, const Type* arg_type, const Type* param_type, Call& call, int arg_index) {
                std::ostringstream oss;

                oss << "call to function " << call.name() << " "
                    << "expected type of argument " << (arg_index + 1) << " to be "
                    << param_type->name()
                    << " but got " << arg_type->name() << " instead";

                return Error(ErrorType::Typing, oss.str(), expression->location());
            }

            Error not_a_function(const Call& call, const Type* type) {
                std::string message = "expected type of " +
                    call.name() + " in call to be " + call.expected_func_type_name() +
                    ", but got " + type->name();

                return Error(ErrorType::Typing, message, call.location());
            }

            Error incorrect_arg_count(const Call& call, const Type* type) {
                std::ostringstream oss;

                // FIX:
                oss << "incorrect argument count for "
                    << call.name()
                    << ": expected " << call.arg_count()
                    << ", but got " << type->name();

                return Error(ErrorType::Typing, oss.str(), call.location());
            }

            // Error return_type_mismatch(const Function* func, const Type* type, const SourceLocation& location) {
            //     auto message = "Trying to return " + type->name() + " from function '" + func->name() + "' returning " + func->return_type()->name();
            //
            //     return Error(ErrorType::Typing, message, location);
            // }

            Error void_return_from_nonvoid_function(const Function* func, const SourceLocation& location) {
                auto message = "Trying to return void from non-void function '" + func->name() + "'";

                return Error(ErrorType::Typing, message, location);
            }
        }

        namespace kir {
            Error moved_variable(Identifier& expr) {
                return Error("cannot reference moved variable", expr.location());
            }
        }
    }

    std::string format_locations(const SourceLocation& start) {
        std::ostringstream oss;
        oss << start.start_line() << ":" << start.start_col();

        if (!start.is_single_col()) {
            oss << "-" << start.end_col();
        }

        return oss.str();
    }

    /* void output_error( */
    /*     const errors::Error& error, */
    /*     const std::string& source_name */
    /* ) { */
    /*     error_indent( */
    /*         "[%s:%s]: %s", */
    /*         source_name.c_str(), */
    /*         format_locations(error.location), */
    /*         error.message.c_str() */
    /*     ); */
    /* } */

    std::string format_error_at_line(
        const std::string& line,
        std::size_t start_col,
        std::size_t end_col
    ) {
        std::ostringstream oss;

        const std::string indent = "|" + std::string(4, ' ');

        if (start_col == end_col) {
            oss << indent << line << std::endl
                << indent << std::setw(end_col + 1) << "^";
        } else {
            oss << indent << line << std::endl
                << indent
                << std::string(start_col + 1, ' ')
                << "^"
                << std::setfill('-')
                << std::string(end_col - start_col, ' ')
                << "^";
        }

        return oss.str();
    }

    std::string format_error_at_line(const std::string& line, const SourceLocation& location) {
        return format_error_at_line(line, location.start_col(), location.end_col());
    }

    std::string format_error(
        const std::string& msg,
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    ) {
        std::ostringstream oss;

        if (start_col == end_col) {
            oss << msg << " (line " << lnum << ", column " << end_col << ")"
                << std::endl;
        } else {
            oss << msg << " (line " << lnum
                << ", columns " << (start_col + 1) << "-" << (end_col + 1) << ")"
                << std::endl;
        }

        format_error_at_line(line, start_col, end_col);

        return oss.str();
    }

    std::string format_error(
        const std::string& msg,
        const std::string& line,
        const SourceLocation& location
    ) {
        return format_error(msg, line, location.start_line(), location.start_col(), location.end_col());
    }

    void throw_error_for_line(
        const std::string& msg,
        const std::string& line,
        std::size_t lnum,
        std::size_t start_col,
        std::size_t end_col
    ) {
        throw std::runtime_error(format_error(msg, line, lnum, start_col, end_col));
    }
}
