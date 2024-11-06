#include "targets/bytecode/codegen/kir/instruction.hpp"

namespace kore {
    namespace kir {
        std::ostream& operator<<(std::ostream& os, const Instruction instruction) {
            auto str_opcode = bytecode_to_string(instruction.opcode);

            os << str_opcode;

            if (auto ins_type = std::get_if<kir::OneRegister>(&instruction.type)) {
                os << " " << ins_type->reg;
            } else if (auto ins_type = std::get_if<kir::TwoRegisters>(&instruction.type)) {
                os << " " << ins_type->reg1 << " " << ins_type->reg2;
            } else if (auto ins_type = std::get_if<kir::ThreeRegisters>(&instruction.type)) {
                os << " "
                   << ins_type->reg1 << " "
                   << ins_type->reg2 << " "
                   << ins_type->reg3;
            } else if (auto ins_type = std::get_if<kir::RegisterAndValue>(&instruction.type)) {
                os << " " << ins_type->reg << " " << ins_type->value;
            } else if (auto ins_type = std::get_if<kir::Value>(&instruction.type)) {
                os << " " << ins_type->value;
            } else if (/*auto ins_type = */std::get_if<kir::CallV>(&instruction.type)) {
            } else if (/*auto ins_type = */std::get_if<kir::ReturnV>(&instruction.type)) {
            }

            return os;
        }
    }
}
