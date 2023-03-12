#include <sstream>

#include "disassemble_error.hpp"

namespace koredis {
    DisassembleError::DisassembleError(const std::string& message) : std::runtime_error(message), _pos(-1) {}

    DisassembleError::DisassembleError(const std::string& message, std::streampos pos) : std::runtime_error(message), _pos(pos) {}

    DisassembleError::~DisassembleError() {}

    const char* DisassembleError::what() const noexcept {
        std::ostringstream oss;

        oss << _message;

        if (_pos != -1) {
            oss << " (at byte position " << _pos << ")";
        }

        return "";
    }
}
