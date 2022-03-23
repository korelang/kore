#include "error.hpp"

namespace kore {
    namespace errors {
        Error::Error(ErrorType error_type, const std::string& message, const Location& location, const Location& end)
            : type(error_type), message(message), location(location), end(end) {
        }

        Error::Error(const std::string& message, const Location& location, const Location& end)
            : type(ErrorType::General), message(message), location(location), end(end) {
        }
    }
}
