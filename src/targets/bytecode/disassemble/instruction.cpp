#include <sstream>
#include "instruction.hpp"

namespace koredis {
    const std::string register_symbol = "@";
    const std::string constant_index_symbol = "#";

    std::ostream& reg(std::ostream& os, kore::Reg reg) {
        return os << register_symbol << reg;
    }

    std::ostream& format_register_list(
        std::ostream& os,
        const std::vector<kore::Reg> registers
    ) {
        for (auto reg : registers) {
            os << " " << register_symbol << reg;
        }

        return os;
    }

    std::ostream& format_registers(std::ostream& os, Instruction instruction) {
        // Special-case output for more complex opcodes
        auto instruction_type = instruction.value.type;

        if (auto ins_type = std::get_if<kore::kir::OneRegister>(&instruction_type)) {
        } else if (auto ins_type = std::get_if<kore::kir::TwoRegisters>(&instruction_type)) {
            if (instruction.value.opcode == kore::Bytecode::Gstore) {
                os << " " << constant_index_symbol << ins_type->reg1 << " " << register_symbol << ins_type->reg2;
            } else {
                os << " " << register_symbol << ins_type->reg1 << " " << register_symbol << ins_type->reg2;
            }
        } else if (auto ins_type = std::get_if<kore::kir::ThreeRegisters>(&instruction_type)) {
            os << " " << register_symbol << ins_type->reg1 << " "
               << register_symbol << ins_type->reg2 << " "
               << register_symbol << ins_type->reg3;
        } else if (auto ins_type = std::get_if<kore::kir::RegisterAndValue>(&instruction_type)) {
            auto opcode = instruction.value.opcode;
            auto value = ins_type->value;
            os << " "; reg(os, ins_type->reg);

            if (opcode == kore::Bytecode::JumpIf || opcode == kore::Bytecode::JumpIfNot) {
                auto target_pos = instruction.byte_pos + value;

                os << " " << ins_type->value << " [target: " << target_pos << "]";
            } else if (opcode == kore::Bytecode::LoadBool) {
                os << " " << (value == 1 ? "true" : "false");
            } else {
                os << " " << constant_index_symbol << value;
            }
        } else if (auto ins_type = std::get_if<kore::kir::Value>(&instruction_type)) {
            auto value = ins_type->value;
            auto target_pos = instruction.byte_pos + value;

            os << " " << value << " [target: " << target_pos << "]";
        } else if (auto ins_type = std::get_if<kore::kir::CallV>(&instruction_type)) {
            os << " " << register_symbol << ins_type->func_index;
            format_register_list(os, ins_type->arg_registers);
            format_register_list(os, ins_type->ret_registers);

            os << " [args: "
               << ins_type->arg_registers.size()
               << ", returns: "
               << ins_type->ret_registers.size() << "]";
        } else if (auto ins_type = std::get_if<kore::kir::ReturnV>(&instruction_type)) {
            format_register_list(os, ins_type->registers);
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Instruction instruction) {
        os << instruction.byte_pos << ": " << bytecode_to_string(instruction.value.opcode);
        
        return format_registers(os, instruction);
    }
}
