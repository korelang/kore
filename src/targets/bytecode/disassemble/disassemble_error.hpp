#ifndef KOREDIS_DISASSEMBLE_ERROR_HPP
#define KOREDIS_DISASSEMBLE_ERROR_HPP

#include <string>
#include <stdexcept>

namespace koredis {
    class DisassembleError final : public std::runtime_error {
        public:
            DisassembleError(const std::string& message);
            DisassembleError(const std::string& message, std::streampos pos);
            DisassembleError(
                const std::string& message,
                std::streampos pos,
                int unknown_opcode
            );
            virtual ~DisassembleError();

            const char* what() const noexcept override;

        private:
            std::string _message;
            std::streampos _pos;
    };
}

#endif // KOREDIS_DISASSEMBLE_ERROR_HPP
