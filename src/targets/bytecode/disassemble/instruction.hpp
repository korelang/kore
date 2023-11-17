#ifndef KOREDIS_INSTRUCTION_HPP
#define KOREDIS_INSTRUCTION_HPP

#include <ostream>
#include <vector>

#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/register.hpp"

namespace koredis {
    class Instruction final {
        public:
            Instruction();
            Instruction(kore::Bytecode opcode, int pos, int byte_pos);
            Instruction(kore::Bytecode opcode, int pos, int byte_pos, kore::Reg reg1);
            Instruction(kore::Bytecode opcode, int pos, int byte_pos, kore::Reg reg1, kore::Reg reg2);
            Instruction(kore::Bytecode opcode, int pos, int byte_pos, kore::Reg reg1, kore::Reg reg2, kore::Reg reg3);
            virtual ~Instruction();

            static Instruction load(kore::Bytecode opcode, int pos, int byte_pos, int reg1, int value);
            static Instruction with_offset(kore::Bytecode opcode, int pos, int byte_pos, int offset);
            static Instruction with_offset(kore::Bytecode opcode, int pos, int byte_pos, int reg1, int offset);
            static Instruction call(
                kore::Bytecode opcode,
                int pos,
                int byte_pos,
                int func_index,
                int return_count,
                int arg_count,
                const std::vector<kore::Reg>& return_registers,
                const std::vector<kore::Reg>& arg_registers
            );
            static Instruction ret(
                kore::Bytecode opcode,
                int pos,
                int byte_pos,
                int return_count,
                const std::vector<kore::Reg>& return_registers
            );

            int position() const;
            int byte_position() const;
            kore::Bytecode opcode() const;
            std::string name() const;
            /* short register_count() const; */
            kore::Reg reg1() const;
            kore::Reg reg2() const;
            kore::Reg reg3() const;
            int value() const;
            std::vector<kore::Reg> return_registers() const;
            std::vector<kore::Reg> call_registers() const;
            std::string registers_as_string() const;

        private:
            int _pos;
            int _byte_pos;
            kore::Bytecode _opcode;
            kore::Reg _reg1;
            kore::Reg _reg2;
            kore::Reg _reg3;

            // Offset for jump instructions or constant table indices for loads
            int _value;

            std::vector<kore::Reg> _return_registers;
            std::vector<kore::Reg> _arg_registers;
    };

    std::ostream& operator<<(std::ostream& os, const Instruction instruction);
}

#endif // KOREDIS_INSTRUCTION_HPP
