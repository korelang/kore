#ifndef KORE_VM_HPP
#define KORE_VM_HPP

#include <vector>

#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/vm/value_type.hpp"
#include "backend/register.hpp"
#include "backend/module.hpp"

namespace kore {
    namespace vm {
        #ifndef KORE_VM_MAX_REGISTERS
        constexpr int KORE_VM_MAX_REGISTERS = 256;
        #else
        constexpr int KORE_VM_MAX_REGISTERS = KORE_VM_MAX_REGISTERS;
        #endif

        /* #ifndef KORE_VM_CALLSTACK_SIZE */
        /*     constexpr int KORE_VM_CALLSTACK_SIZE = 256; */
        /* #endif */

        struct VmContext {
            std::size_t pc = 0; // Program counter
            std::size_t sp = 0; // Stack pointer
            std::size_t fp = 0; // Frame pointer for current call frame
            bytecode_type* code; // Pointer to instructions currently being executed

            void reset() {
                pc = 0, sp = 0, fp = 0;
                code = nullptr;
            }

            /* void set_from_module(const Module& module) { */
            /*     reset(); */
            /*     code = module.main_object().code(0); */
            /* } */
        };

        /// Kore's register-based virtual machine
        class Vm final {
            public:
                Vm();
                ~Vm();

                /// Run the code in a sized array
                void run(const bytecode_type* code, std::size_t size);

                /// Run the code in a vector
                void run(const std::vector<bytecode_type>& code);

                /* void run_module(const Module& module); */

            private:
                VmContext _context;
                std::size_t _pc = 0; // Program counter
                std::size_t _sp = 0; // Stack pointer
                std::size_t _fp = 0; // Frame pointer for current call frame
                Value _registers[KORE_VM_MAX_REGISTERS];
                /* Value _callstack[KORE_VM_CALLSTACK_SIZE]; */

                // Map of loaded modules
                std::unordered_map<std::string, Module> _modules;

            private:
                Bytecode inline decode_opcode(bytecode_type instruction);
                void inline decode_address2_opcode(bytecode_type opcode, int* dest_reg, int* value);
                void inline decode_address3_opcode(bytecode_type opcode, int* dest_reg, int* op1, int* op2);
                void throw_unknown_opcode(Bytecode opcode);
        };
    }
}

#endif // KORE_VM_HPP
