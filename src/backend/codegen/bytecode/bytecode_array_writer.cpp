#include "codegen/bytecode/bytecode_array_writer.hpp"

#include <iostream>

namespace kore {
    static constexpr int JUMP_BITMASK = 0xffff;
    static constexpr int OPCODE_BITMASK = 0xff;
    static constexpr int OPCODE_SHIFT = 24;

    BytecodeArrayWriter::BytecodeArrayWriter() {}

    BytecodeArrayWriter::~BytecodeArrayWriter() {}

    void BytecodeArrayWriter::write_header() {}

    void BytecodeArrayWriter::write_opcode(Bytecode opcode) {
        _code.push_back(static_cast<Bytecode>(opcode << OPCODE_SHIFT));
    }

    void BytecodeArrayWriter::write_1address(Bytecode opcode, Reg reg, i32 value) {
        _code.push_back(static_cast<Bytecode>(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (reg & 0xff) << 16 |
            (value & 0xffff)
        ));
    }

    void BytecodeArrayWriter::write_2address(Bytecode opcode, Reg destination_reg, Reg reg_operand) {
        _code.push_back(static_cast<Bytecode>(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (destination_reg & 0xff) << 16 |
            (reg_operand & 0xff) << 8
        ));
    }

    void BytecodeArrayWriter::write_3address(Bytecode opcode, Reg destination_reg, Reg reg_operand1, Reg reg_operand2) {
        _code.push_back(static_cast<Bytecode>(
            (opcode & OPCODE_BITMASK) << OPCODE_SHIFT |
            (destination_reg & 0xff) << 16 |
            (reg_operand1 & 0xff) << 8 |
            (reg_operand2 & 0xff)
        ));
    }

    void BytecodeArrayWriter::save_patch_location() {
        _patch_locations[_label_count] = _code.size() - 1;
    }

    Label BytecodeArrayWriter::write_jump(Bytecode opcode) {
        write_opcode(opcode);
        save_patch_location();

        return _label_count++;
    }

    Label BytecodeArrayWriter::write_jump(Bytecode opcode, Reg reg) {
        // Write a zero-offset jump that is patched later
        write_1address(opcode, reg, 0);
        save_patch_location();

        return _label_count++;
    }

    void BytecodeArrayWriter::patch_jump(Label label) {
        if (_patch_locations.empty()) {
            throw std::runtime_error("Cannot patch jump without a saved patch location");
        }

        auto patch_pos = _patch_locations[label];
        auto pos = _code.size() - 1;
        auto offset = pos - patch_pos + 1;

        _code[patch_pos] = static_cast<Bytecode>(_code[patch_pos] | (offset & JUMP_BITMASK));
    }

    std::size_t BytecodeArrayWriter::size() const {
        return _code.size();
    }

    code_iterator BytecodeArrayWriter::begin() const {
        return _code.cbegin();
    }

    code_iterator BytecodeArrayWriter::end() const {
        return _code.cend();
    }
}
