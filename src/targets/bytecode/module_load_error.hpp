#ifndef KORE_MODULE_LOAD_ERROR_HPP
#define KORE_MODULE_LOAD_ERROR_HPP

#include <string>
#include <stdexcept>

namespace kore {
    class ModuleLoadError final : public std::runtime_error {
        public:
            ModuleLoadError(const std::string& message);
            ModuleLoadError(const std::string& message, std::streampos pos);
            ModuleLoadError(
                const std::string& message,
                std::streampos pos,
                int unknown_opcode
            );
            virtual ~ModuleLoadError();

            const char* what() const noexcept override;

        private:
            std::string _message;
            std::streampos _pos;
    };
}

#endif // KORE_MODULE_LOAD_ERROR_HPP
