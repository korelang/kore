#ifndef KORE_COMPILED_CODE_HPP
#define KORE_COMPILED_CODE_HPP

#include <string>
#include <vector>

#include "ast/statements/function.hpp"
#include "codegen/bytecode/bytecode.hpp"
#include "location.hpp"
#include "register.hpp"

namespace kore {
    /// A compiled object such as a function or a struct
    class CompiledObject final {
        public:
            using pointer = std::unique_ptr<CompiledObject>;
            using const_pointer = const pointer;

        public:
            friend class BytecodeArrayWriter;
            friend class BytecodeFormatWriter;

            using instruction_iterator = std::vector<bytecode_type>::const_iterator;

        public:
            CompiledObject();
            CompiledObject(const std::string& name);
            CompiledObject(const Function* func);
            virtual ~CompiledObject();

            std::string name() const;
            Location location() const;
            int locals_count() const;
            int reg_count() const;
            Reg allocate_register();
            void free_registers(int count);
            void add_instruction(bytecode_type instruction);
            int code_size() const;
            bool is_main_object() const;

            bytecode_type& operator[](int index);

            instruction_iterator begin() const;
            instruction_iterator end() const;

        private:
            std::string _name;
            Location _location;
            int _local_count = 0;
            std::vector<bytecode_type> _instructions;

            // For now, we just use a very simple register allocator with
            // a maximum of 256 registers that just bumps a register count
            int _reg_count = 0;

            bytecode_type* instructions();
    };
}

#endif // KORE_COMPILED_CODE_HPP
