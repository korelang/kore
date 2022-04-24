#include "codegen/bytecode/bytecode_array_writer.hpp"

namespace kore {
    static constexpr int JUMP_BITMASK = 0xffff;
    static constexpr int OPCODE_BITMASK = 0xff;
    static constexpr int OPCODE_SHIFT = 24;

    BytecodeArrayWriter::BytecodeArrayWriter() {}

    BytecodeArrayWriter::~BytecodeArrayWriter() {}

    void BytecodeArrayWriter::write_header() {}

    void BytecodeArrayWriter::write_opcode(Bytecode opcode, CompiledObject* target) {
        target->add_instruction(opcode << OPCODE_SHIFT);
    }

    void BytecodeArrayWriter::write_load(Bytecode opcode, Reg reg, i32 value, CompiledObject* target) {
        target->add_instruction(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (reg & 0xff) << 16 |
            (value & 0xffff)
        );
    }

    void BytecodeArrayWriter::write_1address(Bytecode opcode, Reg reg, CompiledObject* target) {
        target->add_instruction(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (reg & 0xff) << 16
        );
    }

    void BytecodeArrayWriter::write_2address(Bytecode opcode, Reg destination_reg, Reg reg_operand, CompiledObject* target) {
        target->add_instruction(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (destination_reg & 0xff) << 16 |
            (reg_operand & 0xff) << 8
        );
    }

    void BytecodeArrayWriter::write_3address(Bytecode opcode, Reg destination_reg, Reg reg_operand1, Reg reg_operand2, CompiledObject* target) {
        target->add_instruction(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (destination_reg & 0xff) << 16 |
            (reg_operand1 & 0xff) << 8 |
            (reg_operand2 & 0xff)
        );
    }

    void BytecodeArrayWriter::save_patch_location(CompiledObject* target) {
        _patch_locations[_label_count] = target->code_size() - 1;
    }

    Label BytecodeArrayWriter::write_jump(Bytecode opcode, CompiledObject* target) {
        write_opcode(opcode, target);
        save_patch_location(target);

        return _label_count++;
    }

    Label BytecodeArrayWriter::write_jump(Bytecode opcode, Reg reg, CompiledObject* target) {
        // Write a zero-offset jump that is patched later
        write_load(opcode, reg, 0, target);
        save_patch_location(target);

        return _label_count++;
    }

    void BytecodeArrayWriter::patch_jump(Label label, CompiledObject* target) {
        if (_patch_locations.find(label) == _patch_locations.end()) {
            throw std::runtime_error("Cannot patch non-existing jump");
        }

        auto patch_pos = _patch_locations[label];
        auto pos = target->code_size() - 1;
        auto offset = pos - patch_pos + 1;
        auto patched_code = (*target)[patch_pos] | (offset & JUMP_BITMASK);

        (*target)[patch_pos] = patched_code;
    }
}
