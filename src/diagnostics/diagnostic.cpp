#include "diagnostic.hpp"
#include "ast/expressions/call.hpp"
#include "ast/statements/function.hpp"
#include "ast/statements/return_statement.hpp"
#include "logging/color.hpp"
#include "logging/color_attributes.hpp"
#include "types/function_type.hpp"

#include <type_traits>

namespace kore {
    struct DiagnosticFormatter {
        std::string quote(const std::string& value, char quote = '\'') {
            return quote + value + quote;
        }

        DiagnosticFormatResult operator()(ArrayIndexNotNumeric data) {
            auto message = "array index expression must be numeric but got "
                + data.index_expr->type()->name();

            return { message, data.index_expr->location() };
        }

        DiagnosticFormatResult operator()(ArrayIndexNeedsCast data) {
            auto message = "array index expression must be of type i32 but got "
                + data.index_expr->type()->name()
                + ", consider casting the expression first";

            return { message, data.index_expr->location() };
        }

        DiagnosticFormatResult operator()(CannotAssign data) {
            auto rhs_type = data.assignment->rhs_type(data.lhs_idx);

            auto message = "cannot assign expression of type "
                + rhs_type->name()
                + " to variable of type "
                + data.assignment->lhs(data.lhs_idx)->type()->name()
                + " without conversion";

            return { message, data.assignment->location() };
        }

        DiagnosticFormatResult operator()(IncompatibleBinaryOperation data) {
            auto message = "cannot use binary operator '"
                + binop_to_string(data.op) + "'"
                + " with types "
                + data.left->type()->name()
                + " and "
                + data.right->type()->name()
                + " without conversion";

            return { message, SourceLocation(data.left->location(), data.right->location()) };
        }

        DiagnosticFormatResult operator()(IncompatibleTypes data) {
            auto message = "types "
                + data.expr1->type()->name()
                + " and "
                + data.expr2->type()->name()
                + " are incompatible";

            return { message, data.expr1->location() };
        }

        DiagnosticFormatResult operator()(ReturnTypeMismatch data) {
            auto message = "trying to return "
                + data.ret->get_expr(data.idx)->type()->name()
                + " from function " + quote(data.function->name())
                + " returning " + data.function->type()->return_type(data.idx)->name();

            return { message, data.ret->location() };
        }

        DiagnosticFormatResult operator()(VoidReturnFromNonVoidFunction data) {
            auto message = "cannot return void from non-void function "
                + quote(data.function->name());

            return { message, data.ret->location() };
        }

        DiagnosticFormatResult operator()(UnknownCall data) {
            auto message = "unknown function call "
                + quote(data.call->name());

            return { message, data.call->location() };
        }

        DiagnosticFormatResult operator()(CannotDeclareMutableGlobal data) {
            auto message = "cannot declare global mutable variables ('"
                + data.identifier->name()
                + "')";

            return { message, data.identifier->location() };
        }

        DiagnosticFormatResult operator()(CannotAssignGlobalVariable data) {
            auto message = "cannot assign to global variable previously declared at "
                + data.global_identifier->location().colon_format();

            return { message, data.identifier->location() };
        }

        DiagnosticFormatResult operator()(VariableShadows data) {
            auto identifier = data.identifier;
            auto shadowed_identifier = data.shadowed_identifier;

            auto message = "variable "
                + quote(identifier->name())
                + " shadows variable "
                + quote(shadowed_identifier->name())
                + " declared at "
                + shadowed_identifier->location().colon_format();

            return { message, identifier->location() };
        }

        DiagnosticFormatResult operator()(ConstantVariableRedeclaration data) {
            auto message = "redeclaration of constant variable '"
                + data.identifier->name()
                + "', previously declared at "
                + data.previous_identifier->location().colon_format();

            return { message, data.identifier->location() };
        }

        DiagnosticFormatResult operator()(UndefinedVariable data) {
            auto message = "use of undefined variable " + quote(data.identifier->name());

            return { message, data.identifier->location() };
        }

        DiagnosticFormatResult operator()(CannotCallNonFunction data) {
            auto message = "expected type of " +
                data.call->name()
                + " in call to be " + data.call->expected_func_type_name()
                + ", but got " + data.given_type->name();

            return { message, data.call->location() };
        }

        DiagnosticFormatResult operator()(IncorrectArgumentCount data) {
            auto message = "incorrect argument count for function "
                + data.call->name()
                + ": expected " + std::to_string(data.call->arg_count())
                + ", but got " + data.func_type->name();

            return { message, data.call->location() };
        }

        DiagnosticFormatResult operator()(IncorrectArgumentType data) {
            auto message = "call to function " + data.call->name()
                + " expected type of argument "
                + std::to_string(data.paramter_idx + 1)
                + " to be "
                + data.parameter_type->name()
                + " but got "
                + data.arg->type()->name()
                + " instead";

            return { message, data.call->location() };
        }
    };

    // Format the context of the diagnostic such as the line and surrounding
    // lines where an error occurred
    struct DiagnosticContextFormatter {};

    struct DiagnosticAdviceFormatter {};

    Diagnostic::Diagnostic(DiagnosticType type, DiagnosticLevel level, DiagnosticData data)
        : type(type),
          level(level),
          data(data) {}

    DiagnosticFormatResult Diagnostic::message() const {
        return std::visit(DiagnosticFormatter{}, data);
    }

    std::string Diagnostic::level_string() const {
        switch (level) {
            case DiagnosticLevel::Debug:    return "debug";
            case DiagnosticLevel::Trace:    return "trace";
            case DiagnosticLevel::Info:     return "info";
            case DiagnosticLevel::Warning:  return "warning";
            case DiagnosticLevel::Error:    return "error";
            case DiagnosticLevel::Critical: return "critical";
        }
    }

    std::string Diagnostic::group() const {
        switch (type) {
            case DiagnosticType::General:       return "general";
            case DiagnosticType::Scan:          return "scan";
            case DiagnosticType::Parser:        return "parser";
            case DiagnosticType::TypeInference: return "type_inference";
            case DiagnosticType::TypeCheck:     return "typecheck";
            case DiagnosticType::Codegen:       return "codegen";
        }
    }

    const ColorSpec debug_spec{ Color::Cyan, ColorAttribute::Bold };
    const ColorSpec trace_spec{ Color::Cyan, ColorAttribute::Bold };
    const ColorSpec info_spec{ Color::Cyan, ColorAttribute::Bold };
    const ColorSpec warning_spec{ Color::Cyan, ColorAttribute::Bold };
    const ColorSpec error_spec{ Color::Cyan, ColorAttribute::Bold };
    const ColorSpec critical_spec{ Color::Cyan, ColorAttribute::Bold };

    ColorSpec Diagnostic::color_spec() const {
        switch (level) {
            case DiagnosticLevel::Debug:    return debug_spec;
            case DiagnosticLevel::Trace:    return trace_spec;
            case DiagnosticLevel::Info:     return info_spec;
            case DiagnosticLevel::Warning:  return warning_spec;
            case DiagnosticLevel::Error:    return error_spec;
            case DiagnosticLevel::Critical: return critical_spec;
        }
    }

    int Diagnostic::code() const {
        auto idx = static_cast<std::underlying_type_t<DiagnosticType>>(type);

        return idx;
    }

    std::string Diagnostic::advice() const {
        return "";
    }
}
