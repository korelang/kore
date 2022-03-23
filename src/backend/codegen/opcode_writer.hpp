#ifndef KORE_OPCODE_WRITER_HPP
#define KORE_OPCODE_WRITER_HPP

#include <string>

#include "backend/register.hpp"
#include "internal_value_types.hpp"

namespace kore {
    template<typename Opcode>
    class OpcodeWriter {
        public:
            virtual void write_header() = 0;
            virtual void write(Opcode opcode) = 0;
            /* virtual void write(const char* const bytes, std::size_t count) = 0; */
            /* virtual void write(Opcode opcode, i32 value) = 0; */
            /* virtual void write(Opcode opcode, bool value) = 0; */
            virtual void write(Opcode opcode, Reg reg, i32 value) = 0;
            virtual void write(Opcode opcode, Reg destination_reg, Reg reg_operand1, Reg reg_operand2) = 0;
            virtual void save_patch_location() = 0;
            virtual void write_jump(Opcode opcode, Reg reg) = 0;
            virtual void patch_jump() = 0;

            virtual bool is_dumper() const noexcept {
                return false;
            }
    };
}

#endif // KORE_OPCODE_WRITER_HPP
