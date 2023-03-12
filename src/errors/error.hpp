#ifndef KORE_ERROR_HPP
#define KORE_ERROR_HPP

#include "ast/source_location.hpp"

namespace kore {
    namespace errors {
        enum class ErrorType {
            General,
            Scan,
            Parser,
            Typing,
            Codegen,
        };

        struct Error {
            Error(ErrorType error_type, const std::string& message, const SourceLocation& location, const SourceLocation& end = SourceLocation::unknown);
            Error(const std::string& message, const SourceLocation& location, const SourceLocation& end = SourceLocation::unknown);

            ErrorType type;
            std::string message;

            SourceLocation location;

            // Used for e.g. multiple declaration errors to point to the
            // previous declaration
            SourceLocation end;
        };

        namespace scan {
            enum class Errors {
                CharacterShouldBeString,
                EndOfString,
                EofCodepoint,
                ExpectEqualAfterBang,
                InvalidBinNumber,
                InvalidDecNumberDots,
                InvalidHexNumber,
                InvalidOctNumber,
                InvalidUnicode,
                UnknownCharacter,
            };

            Error character_should_be_string(std::size_t col, std::size_t lnum);
        }
    }

}

#endif // KORE_ERROR_HPP
