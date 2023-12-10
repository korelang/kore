#ifndef KORE_COMPILED_CODE_HPP
#define KORE_COMPILED_CODE_HPP

#include <string>
#include <vector>

#include "ast/source_location.hpp"
#include "ast/statements/function.hpp"
#include "targets/bytecode/register.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "pointer_types.hpp"

namespace kore {
    /// A compiled object such as a function or a struct
    class CompiledObject final {
        public:
            using pointer = Owned<CompiledObject>;
            using const_pointer = const pointer;

        public:
            friend class BytecodeArrayWriter;
            friend class BytecodeFormatWriter;

            using instruction_iterator = std::vector<bytecode_type>::const_iterator;

        public:
            CompiledObject();
            CompiledObject(const std::string& name, int func_index);
            CompiledObject(const Function* func, int func_index);
            CompiledObject(
                const std::string& name,
                const SourceLocation& location,
                int func_index,
                int locals_count,
                int reg_count,
                const std::vector<bytecode_type>& instructions
            );
            virtual ~CompiledObject();

            std::string name() const;
            int func_index() const;
            SourceLocation location() const;
            int locals_count() const;
            int reg_count() const;
            int max_regs_used() const;
            Reg allocate_register();
            void free_registers(int count);
            void add_instruction(bytecode_type instruction);
            int code_size() const;
            bool is_main_object() const;

            bytecode_type& operator[](int index);
            const bytecode_type& operator[](int index) const;

            instruction_iterator begin() const;
            instruction_iterator end() const;
            const bytecode_type* instructions() const;

        private:
            std::string _name;
            int _func_index = -1;
            SourceLocation _location;
            int _local_count = 0;
            std::vector<bytecode_type> _instructions;

            // TODO: Add a pointer to the containing module here

            // For now, we just use a very simple register allocator with
            // a maximum of 256 registers that just bumps a register count
            int _reg_count = 0;

            int _max_regs_used = 0;
    };
}

#endif // KORE_COMPILED_CODE_HPP
