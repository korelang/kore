#ifndef KORE_BYTECODE_ARRAY_WRITER_HPP
#define KORE_BYTECODE_ARRAY_WRITER_HPP

#include <map>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "codegen/compiled_object.hpp"
#include "register.hpp"

namespace kore {
    using Label = int;

    class BytecodeArrayWriter final {
        public:
            BytecodeArrayWriter();
            ~BytecodeArrayWriter();

            void write_header();
            void write_opcode(Bytecode opcode, CompiledObject* target);
            void write_load(Bytecode opcode, Reg reg, i32 value, CompiledObject* target);
            void write_1address(Bytecode opcode, Reg reg, CompiledObject* target);
            void write_2address(Bytecode opcode, Reg destination_reg, Reg reg_operand, CompiledObject* target);
            void write_3address(Bytecode opcode, Reg destination_reg, Reg reg_operand1, Reg reg_operand2, CompiledObject* target);
            void save_patch_location(CompiledObject* target);
            Label write_jump(Bytecode opcode, CompiledObject* target);
            Label write_jump(Bytecode opcode, Reg reg, CompiledObject* target);
            void patch_jump(Label label, CompiledObject* target);

        private:
            Label _label_count = 0;
            std::map<Label, int> _patch_locations;
    };
}

#endif // KORE_BYTECODE_ARRAY_WRITER_HPP
