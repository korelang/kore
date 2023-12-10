#include <sstream>

#include "targets/bytecode/module_load_error.hpp"

namespace kore {
    ModuleLoadError::ModuleLoadError(const std::string& message) : ModuleLoadError(message, -1) {
    }

    ModuleLoadError::ModuleLoadError(const std::string& message, std::streampos pos)
        : ModuleLoadError(message, pos, -1) {
    }

    ModuleLoadError::ModuleLoadError(
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

    ModuleLoadError::~ModuleLoadError() {}

    const char* ModuleLoadError::what() const noexcept {
        return _message.c_str();
    }
}
