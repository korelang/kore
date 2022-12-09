#ifndef KORE_VM_HPP
#define KORE_VM_HPP

#include <filesystem>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "vm/value_type.hpp"
#include "vm/config.hpp"
#include "register.hpp"
#include "module.hpp"

/* namespace fs = std::filesystem; */

namespace kore {
    namespace vm {
        struct Context {
            std::size_t pc = 0; // Program counter
            std::size_t sp = 0; // Stack pointer
            std::size_t fp = 0; // Frame pointer for current call frame

            /* Module* _current_module; */

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
                void throw_unknown_opcode(Bytecode opcode);

                /// Get a function by its index
                int get_function(int func_index);

                /// Get the current call frame
                CallFrame& current_frame();

                /// Push an i32 value onto a call frame's stack
                inline void push_i32(i32 value);

                /// Push a Value onto a call frame's stack
                inline void push(Value& value);

                /// Push the value of a register onto the current call frame
                inline void push_register(Reg reg);

                /// Push a new call frame
                void push_call_frame(bytecode_type instruction);

                /// Pop the current call frame
                void pop_call_frame(bytecode_type instruction);

                /// Pop a value from a call frame's stack
                inline Value pop();

                /// Get the position of the top of the stack
                inline int top();

                /// Move a value from a source register to a destination register
                inline void move(Reg src_reg, Reg dst_reg);

                void add_module(Module& module);
        };
    }
}

#endif // KORE_VM_HPP
