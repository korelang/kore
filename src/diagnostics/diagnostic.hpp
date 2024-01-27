#ifndef KORE_DIAGNOSTIC_HPP
#define KORE_DIAGNOSTIC_HPP

#include "ast/source_location.hpp"
#include "diagnostics/diagnostics.hpp"
#include "logging/color.hpp"
#include "logging/color_spec.hpp"

#include <variant>

namespace kore {
    enum class DiagnosticType {
        General = 0,
        Scan,
        Parser,
        TypeInference,
        TypeCheck,
        Codegen,
    };

    enum class DiagnosticLevel {
        Debug = 0, // Information related to debugging
        Trace, // E.g. tracing the execution of the compiler
        Info, // General informational messages such as things controlled by verbosity
        Warning,
        Error,
        Critical,
    };

    using DiagnosticData = std::variant<
        ArrayIndexNeedsCast,
        ArrayIndexNotNumeric,
        CannotAssign,
        CannotAssignGlobalVariable,
        CannotCallNonFunction,
        CannotDeclareMutableGlobal,
        ConstantVariableRedeclaration,
        IncompatibleBinaryOperation,
        IncompatibleTypes,
        IncorrectArgumentCount,
        IncorrectArgumentType,
        ReturnTypeMismatch,
        UndefinedVariable,
        UnknownCall,
        VariableShadows,
        VoidReturnFromNonVoidFunction
    >;

    using DiagnosticFormatResult = std::pair<std::string, SourceLocation>;

    /// A diagnostic message emitted the compiler passes
    struct Diagnostic {
        Diagnostic(DiagnosticType type, DiagnosticLevel level, DiagnosticData data);

        DiagnosticType type = DiagnosticType::General;
        DiagnosticLevel level = DiagnosticLevel::Info;
        DiagnosticData data;

        DiagnosticFormatResult message() const;
        std::string level_string() const;
        std::string group() const;
        ColorSpec color_spec() const;

        /// An integer code associated with this diagnostic
        int code() const;

        /// Get advice related to resolving the diagnostic
        std::string advice() const;
    };
}

#endif // KORE_DIAGNOSTIC_HPP
