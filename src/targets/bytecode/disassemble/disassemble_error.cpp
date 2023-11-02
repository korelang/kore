#include <sstream>

#include "disassemble_error.hpp"

namespace koredis {
    DisassembleError::DisassembleError(const std::string& message) : DisassembleError(message, -1) {
    }

    DisassembleError::DisassembleError(const std::string& message, std::streampos pos)
        : DisassembleError(message, pos, -1) {
    }

    DisassembleError::DisassembleError(
        const std::string& message,
        std::streampos pos,
        int unknown_opcode
    ) : std::runtime_error(message), _pos(pos) {
        std::ostringstream oss;

        oss << message;

        if (unknown_opcode != -1) {
            oss << ": '" << unknown_opcode << "'";
        }

        if (_pos != -1) {
            oss << " (at byte position " << _pos << ")";
        }

        _message = oss.str();
    }

    DisassembleError::~DisassembleError() {}

    const char* DisassembleError::what() const noexcept {
        return _message.c_str();
    }
}
