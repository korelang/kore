#include <iostream>
#include <vector>

#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/disassemble/decode_instruction.hpp"
#include "targets/bytecode/disassemble/disassemble_error.hpp"
#include "targets/bytecode/disassemble/instruction.hpp"
#include "targets/bytecode/register.hpp"
#include "utils/unused_parameter.hpp"

namespace koredis {
    // TODO: Check byte_offset range
    std::vector<kore::Reg> decode_registers(int& pos, int& byte_offset, int count, const kore::CompiledObject& obj) {
        int i = 0;
        std::vector<kore::Reg> registers;
        kore::bytecode_type ins = obj[pos];

        while (i++ < count) {
            registers.push_back((ins >> (24 - 8 * byte_offset++)) & 0xff);

            if (byte_offset == 4) {
                ins = obj[++pos];
                byte_offset = 0;
            }
        }

        return registers;
    }

    Instruction decode_instruction(
        int& pos,
        int& byte_pos,
        const kore::CompiledObject& obj
    ) {
        kore::bytecode_type instruction = obj[pos];
        auto opcode = GET_OPCODE(instruction);
        Instruction decoded_instruction;

        switch (opcode) {
            case kore::Bytecode::Noop: {
                decoded_instruction = Instruction(opcode, pos++, byte_pos);
                byte_pos += 4;
                break;
            }

            case kore::Bytecode::LoadBool:
            case kore::Bytecode::CloadI32:
            case kore::Bytecode::CloadI64:
            case kore::Bytecode::CloadF32:
            case kore::Bytecode::CloadF64:
            case kore::Bytecode::Gload: {
                decoded_instruction = Instruction::load(
                    opcode,
                    pos++,
                    byte_pos,
                    GET_REG1(instruction),
                    GET_VALUE(instruction)
                );
                byte_pos += 4;
                break;
            }

            case kore::Bytecode::Move:
            case kore::Bytecode::Gstore:
            case kore::Bytecode::AllocArray: {
                decoded_instruction = Instruction(
                    opcode,
                    pos++,
                    byte_pos,
                    GET_REG1(instruction),
                    GET_REG2(instruction)
                );
                byte_pos += 4;
                break;
            }

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
            case kore::Bytecode::ArrayIndexGet:
            case kore::Bytecode::ArrayIndexSet: {
                decoded_instruction = Instruction(
                    opcode,
                    pos++,
                    byte_pos,
                    GET_REG1(instruction),
                    GET_REG2(instruction),
                    GET_REG3(instruction)
                );
                byte_pos += 4;
                break;
            }

            case kore::Bytecode::Jump: {
                decoded_instruction = Instruction::with_offset(
                    opcode,
                    pos++,
                    byte_pos,
                    GET_OFFSET(instruction)
                );
                byte_pos += 4;
                break;
            }

            case kore::Bytecode::JumpIf:
            case kore::Bytecode::JumpIfNot: {
                decoded_instruction = Instruction::with_offset(
                    opcode,
                    pos++,
                    byte_pos,
                    GET_REG1(instruction),
                    GET_OFFSET(instruction)
                );
                byte_pos += 4;
                break;
            }

            case kore::Bytecode::Call: {
                int func_reg = GET_REG1(instruction);
                int arg_count = GET_REG2(instruction);
                int return_count = GET_REG3(instruction);
                int start_pos = pos;
                int byte_offset = 0;
                std::vector<kore::Reg> arg_regs;
                std::vector<kore::Reg> ret_regs;

                arg_regs = decode_registers(pos, byte_offset, arg_count, obj);
                ret_regs = decode_registers(pos, byte_offset, return_count, obj);

                if (arg_count > 0 || return_count > 0) {
                    ++pos;
                }

                decoded_instruction = Instruction::call(
                    opcode,
                    start_pos,
                    byte_pos,
                    func_reg,
                    return_count,
                    arg_count,
                    ret_regs,
                    arg_regs
                );

                byte_pos += 4 + byte_offset;
                break;
            }

            case kore::Bytecode::Ret: {
                int regs = GET_REG1(instruction);
                int byte_offset = 2;
                auto ret_regs = decode_registers(pos, byte_offset, regs, obj);

                decoded_instruction = Instruction::ret(opcode, pos++, byte_pos, regs, ret_regs);
                byte_pos += std::max(byte_offset, 4);
                break;
            }

            default:
                throw DisassembleError("Failed to decode instruction", pos);
        }

        return decoded_instruction;
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

        for (int pos = 0, byte_pos = 0; pos < compiled_object.code_size();) {
            instructions.push_back(decode_instruction(pos, byte_pos, compiled_object));
        }

        return instructions;
    }
}
