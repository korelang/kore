#ifndef KORE_VM_HPP
#define KORE_VM_HPP

#include <filesystem>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "vm/value_type.hpp"
#include "vm/config.hpp"
#include "register.hpp"
#include "module.hpp"

namespace fs = std::filesystem;

namespace kore {
    namespace vm {
        struct Context {
            std::size_t pc = 0; // Program counter
            std::size_t sp = 0; // Stack pointer
            std::size_t fp = 0; // Frame pointer for current call frame

            void reset();

            void restore(int registers, std::size_t fp, std::size_t pc);

            /* void set_from_module(const Module& module) { */
            /*     reset(); */
            /*     code = module.main_object().code(0); */
            /* } */
        };

        struct CallFrame {
            int ret_count;
            int reg_count;
            std::size_t fp;
            const bytecode_type* code; // Pointer to instructions currently being executed
            std::size_t size;
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

                /// Run a compiled object
                /* void run(const CompiledObject* obj) */

                /* void run_path(const fs::path& path); */
                /* void run_module(Module& module); */

            private:
                Context _context;
                Value _registers[KORE_VM_MAX_REGISTERS];
                std::vector<CallFrame> _call_frames;

                // Map of loaded modules
                std::unordered_map<std::string, Module> _modules;

            private:
                Bytecode inline decode_opcode(bytecode_type instruction);
                void inline decode_address2_opcode(bytecode_type opcode, int* dest_reg, int* value);
                void inline decode_address3_opcode(bytecode_type opcode, int* dest_reg, int* op1, int* op2);
                void throw_unknown_opcode(Bytecode opcode);

                /// Get a function by its index
                int get_function(int func_index);

                /// Get the current call frame
                CallFrame& current_frame();

                /// Push an i32 value onto a call frame's stack
                void push(i32 value);

                /// Push a Value onto a call frame's stack
                void push(Value& value);

                /// Push the value of a register onto the current call frame
                void push_register(Reg reg);

                /// Push a new call frame
                void push_call_frame(bytecode_type instruction);

                /// Pop the current call frame
                void pop_call_frame();

                /// Pop a value from a call frame's stack
                Value pop();

                /// Get the position of the top of the stack
                inline int top();

                /// Move a value from a source register to a destination register
                void move(Reg src_reg, Reg dst_reg);

                void add_module(Module& module);
        };
    }
}

#endif // KORE_VM_HPP
