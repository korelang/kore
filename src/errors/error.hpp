#ifndef KORE_ERROR_HPP
#define KORE_ERROR_HPP

#include "location.hpp"

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
            Error(ErrorType error_type, const std::string& message, const Location& location, const Location& end = Location::unknown);
            Error(const std::string& message, const Location& location, const Location& end = Location::unknown);

            ErrorType type;
            std::string message;

            Location location;

            // Used for e.g. multiple declaration errors to point to the
            // previous declaration
            Location end;
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
