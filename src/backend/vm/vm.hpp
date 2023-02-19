#ifndef KORE_VM_HPP
#define KORE_VM_HPP

#include <cstddef>
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
        struct CallFrame {
            CallFrame(int ret_count, int reg_count, const bytecode_type* code, std::size_t size);
            CallFrame(int ret_count, int shift, std::size_t old_fp, std::size_t old_pc, CompiledObject* obj);

            int ret_count;
            int reg_count;
            int shift;
            std::size_t old_fp;
            std::size_t old_pc;

            // Pointer to instructions currently being executed
            const bytecode_type* code;
            std::size_t size;
        };

        struct Context {
            std::size_t pc = 0; // Program counter
            std::size_t sp = 0; // Stack pointer
            std::size_t fp = 0; // Frame pointer for current call frame

            Module* _current_module;

            void reset();
            void restore(const CallFrame& call_frame);

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

                /// Run a compiled object
                /* void run(const CompiledObject* obj) */

                void run_path(const fs::path& path);
                void run_module(Module& module);
                void run_compiled_object(CompiledObject* obj);

                void dump_registers(std::ostream& os);

            private:
                Context _context;
                Value _registers[KORE_VM_MAX_REGISTERS];
                std::vector<CallFrame> _call_frames;

                // Saved for dumping the main function's registers
                int main_call_frame_reg_count;

                // Map of loaded modules
                std::unordered_map<std::string, Module> _modules;

                // Vector of loaded functions from all loaded modules for quick access
                std::vector<CompiledObject*> _loaded_functions;

                // Table of global values
                std::vector<Value> _globals;

            private:
                /// Load all functions from a module
                void load_functions_from_module(const Module& module);

                void throw_unknown_opcode(Bytecode opcode);

                /// Get a function by its index
                CompiledObject* get_function(int func_index);

                /// Get the current call frame
                CallFrame& current_frame();

                /// Push an i32 value onto a call frame's stack
                inline void push_i32(i32 value);

                /// Push a Value onto a call frame's stack
                inline void push(Value& value);

                /// Push the value of a register onto the current call frame
                inline void push_register(Reg reg);

                /// When in a function call, get the caller of that function
                inline const CallFrame& get_caller();

                /// Push a new call frame
                void do_call(bytecode_type instruction);

                /// Pop the current call frame
                void do_return(bytecode_type instruction);

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
