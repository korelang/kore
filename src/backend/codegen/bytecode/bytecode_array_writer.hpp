#ifndef KORE_BYTECODE_ARRAY_WRITER_HPP
#define KORE_BYTECODE_ARRAY_WRITER_HPP

#include <map>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "codegen/opcode_writer.hpp"
#include "register.hpp"

namespace kore {
    using code_iterator = std::vector<Bytecode>::const_iterator;
    using Label = int;

    class BytecodeArrayWriter final {
        public:
            BytecodeArrayWriter();
            ~BytecodeArrayWriter();

            void write_header();
            void write_opcode(Bytecode opcode);
            void write_1address(Bytecode opcode, Reg reg, i32 value);
            void write_2address(Bytecode opcode, Reg destination_reg, Reg reg_operand);
            void write_3address(Bytecode opcode, Reg destination_reg, Reg reg_operand1, Reg reg_operand2);
            void save_patch_location();
            Label write_jump(Bytecode opcode);
            Label write_jump(Bytecode opcode, Reg reg);
            void patch_jump(Label label);

            std::size_t size() const;
            code_iterator begin() const;
            code_iterator end() const;

        private:
            Label _label_count = 0;
            std::vector<Bytecode> _code;
            std::map<Label, int> _patch_locations;
    };
}

#endif // KORE_BYTECODE_ARRAY_WRITER_HPP
