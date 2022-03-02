#ifndef KORE_VM_HPP
#define KORE_VM_HPP

#include <vector>

#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/vm/value_type.hpp"

namespace kore {
    #ifndef KORE_VM_MAX_REGISTERS
        constexpr int KORE_VM_MAX_REGISTERS = 256;
    #else
        constexpr int KORE_VM_MAX_REGISTERS = KORE_VM_MAX_REGISTERS;
    #endif

    /// Kore's register-based virtual machine
    class Vm final {
        public:
            Vm();
            ~Vm();

            /// Run the code in a sized array
            void run(const bytecode_type* code, std::size_t size);

            /// Run the code in a vector
            void run(const std::vector<bytecode_type>& code);

        private:
            std::size_t _pc = 0; // Program counter
            Value _registers[KORE_VM_MAX_REGISTERS];

            Bytecode inline decode_opcode(bytecode_type instruction);
            void inline decode_address2_opcode(bytecode_type opcode, int* dest_reg, int* value);
            void inline decode_address3_opcode(bytecode_type opcode, int* dest_reg, int* op1, int* op2);
    };
}

#endif // KORE_VM_HPP
