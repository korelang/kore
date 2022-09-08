#include <iostream>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "decode_instruction.hpp"
#include "instruction.hpp"
#include "register.hpp"

namespace koredis {
    std::vector<kore::Reg> decode_call_registers(int arg_count, const kore::CompiledObject& obj, int& pos) {
        // We start on the second register position since the three first are
        // the opcode, the arg count, and the return register
        int arg = 2;
        std::vector<kore::Reg> call_registers;
        kore::bytecode_type ins = obj[pos];

        while (arg <= arg_count) {
            int reg_pos = arg % 4;
            kore::Reg reg = GET_REG(ins, reg_pos);
            call_registers.push_back(reg);

            if (reg == 3) {
                ins = obj[pos++];
            }

            ++arg;
        }

        return call_registers;
    }

    Instruction decode_instruction(int& pos, const kore::CompiledObject& obj) {
        kore::bytecode_type instruction = obj[pos];
        auto opcode = static_cast<kore::Bytecode>(instruction >> 24 & 0xff);

        switch (opcode) {
            case kore::Bytecode::Noop:
                return Instruction(opcode, pos++);

            case kore::Bytecode::LoadBool:
            case kore::Bytecode::CloadI32:
            case kore::Bytecode::CloadI64:
            case kore::Bytecode::CloadF32:
            case kore::Bytecode::CloadF64:
                return Instruction::load(opcode, pos++, GET_REG1(instruction), GET_VALUE(instruction));

            case kore::Bytecode::Move:
            case kore::Bytecode::StoreI32Global:
                return Instruction(
                    opcode,
                    pos++,
                    GET_REG1(instruction),
                    GET_REG2(instruction)
                );

            case kore::Bytecode::AddI32:
            case kore::Bytecode::AddI64:
            case kore::Bytecode::AddF32:
            case kore::Bytecode::AddF64:
            case kore::Bytecode::SubI32:
            case kore::Bytecode::SubI64:
            case kore::Bytecode::SubF32:
            case kore::Bytecode::SubF64:
            case kore::Bytecode::MultI32:
            case kore::Bytecode::MultI64:
            case kore::Bytecode::MultF32:
            case kore::Bytecode::MultF64:
            case kore::Bytecode::PowI32:
            case kore::Bytecode::PowI64:
            case kore::Bytecode::PowF32:
            case kore::Bytecode::PowF64:
            case kore::Bytecode::DivI32:
            case kore::Bytecode::DivI64:
            case kore::Bytecode::DivF32:
            case kore::Bytecode::DivF64:
            case kore::Bytecode::LtI32:
            case kore::Bytecode::LtI64:
            case kore::Bytecode::LtF32:
            case kore::Bytecode::LtF64:
            case kore::Bytecode::GtI32:
            case kore::Bytecode::GtI64:
            case kore::Bytecode::GtF32:
            case kore::Bytecode::GtF64:
            case kore::Bytecode::LeI32:
            case kore::Bytecode::LeI64:
            case kore::Bytecode::LeF32:
            case kore::Bytecode::LeF64:
            case kore::Bytecode::GeI32:
            case kore::Bytecode::GeI64:
            case kore::Bytecode::GeF32:
            case kore::Bytecode::GeF64:
            case kore::Bytecode::EqI32:
            case kore::Bytecode::EqI64:
            case kore::Bytecode::EqF32:
            case kore::Bytecode::EqF64:
            case kore::Bytecode::NeqI32:
            case kore::Bytecode::NeqI64:
            case kore::Bytecode::NeqF32:
            case kore::Bytecode::NeqF64:
                return Instruction(
                    opcode,
                    pos++,
                    GET_REG1(instruction),
                    GET_REG2(instruction),
                    GET_REG3(instruction)
                );

            case kore::Bytecode::Jump:
                return Instruction(opcode, pos++, GET_REG1(instruction));

            case kore::Bytecode::Call: {
                int arg_count = GET_ARG_COUNT(instruction);
                int start_pos = pos;
                pos += 1 + arg_count;

                return Instruction::call(
                    opcode,
                    start_pos,
                    GET_RET_REG(instruction),
                    decode_call_registers(arg_count, obj, start_pos)
                );
            }

            case kore::Bytecode::JumpIf:
            case kore::Bytecode::JumpIfNot:
                return Instruction::with_offset(
                    opcode,
                    pos++,
                    GET_REG1(instruction),
                    GET_OFFSET(instruction)
                );

            case kore::Bytecode::Ret:
                return Instruction(opcode, pos++);

            case kore::Bytecode::RetReg:
                return Instruction(opcode, pos++, GET_REG1(instruction));

            default:
                return Instruction(kore::Bytecode::Noop, pos++);
        }
    }

    /* void decode_instruction(std::ostream& os, Instruction ins, int pos, int register_count) { */
    /*     os << pos << spacing << bytecode_to_string(GET_OPCODE(ins)); */

    /*     for (int i = register_count - 1; i >= 0; ++i) { */
    /*         os << "\t" << (GET_REG(ins, i)); */
    /*     } */

    /*     os << std::endl; */
    /* } */

    std::vector<Instruction> decode_instructions(const kore::CompiledObject compiled_object) {
        std::vector<Instruction> instructions;

        for (int pos = 0; pos < compiled_object.code_size();) {
            instructions.push_back(decode_instruction(pos, compiled_object));
        }

        return instructions;
    }
}
