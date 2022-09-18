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

    void BytecodeArrayWriter::write_bytes(const std::vector<std::uint8_t>& bytes, CompiledObject* target) {
        std::size_t i = 0;

        // Pack every four bytes into a 32-bit integer
        for (; i < bytes.size() / 4; ++i) {
            int idx = i * 4;

            bytecode_type int32 =
                (bytes[idx + 0] << 24)
                | (bytes[idx + 1] << 16)
                | (bytes[idx + 2] << 8)
                | (bytes[idx + 3] << 0);

            target->add_instruction(int32);
        }

        int remaining_bytes = bytes.size() % 4;

        // Pack any remaining (<4) bytes into a 32-bit big-endian integer
        // padded with zeroes
        if (remaining_bytes > 0) {
            int idx = i * 4;

            bytecode_type int32 = 0;
            int32 |= bytes[idx] << 24;

            if (remaining_bytes > 1) {
                int32 |= bytes[idx + 1] << 16;
            }

            if (remaining_bytes > 2) {
                int32 |= bytes[idx + 2] << 8;
            }

            target->add_instruction(int32);
        }
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
