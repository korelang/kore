#include <iostream>
#include <vector>

#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "targets/bytecode/disassemble/decode_instruction.hpp"
#include "targets/bytecode/disassemble/instruction.hpp"
#include "targets/bytecode/module_load_error.hpp"
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
        bool byte_pos_advanced = false;

        switch (opcode) {
            case kore::Bytecode::Noop: {
                // We just use the OneRegister variant here to avoid having
                // another variant for an opcode with no arguments
                decoded_instruction = Instruction{
                    pos++,
                    byte_pos,
                    { opcode, kore::kir::OneRegister{ kore::INVALID_REGISTER } }
                };
                break;
            }

            case kore::Bytecode::LoadBool:
            case kore::Bytecode::Cload:
            case kore::Bytecode::LoadFunction:
            case kore::Bytecode::Gload:
            case kore::Bytecode::JumpIf:
            case kore::Bytecode::JumpIfNot: {
                decoded_instruction = Instruction{
                    pos++,
                    byte_pos,
                    {
                        opcode,
                        kore::kir::RegisterAndValue{
                            static_cast<kore::Reg>(GET_REG1(instruction)),
                            static_cast<kore::Reg>(GET_VALUE(instruction))
                        }
                    }
                };
                break;
            }

            case kore::Bytecode::Move:
            case kore::Bytecode::Gstore:
            case kore::Bytecode::ArrayAlloc: {
                decoded_instruction = Instruction{
                    pos++,
                    byte_pos,
                    {
                        opcode,
                        kore::kir::TwoRegisters{
                            static_cast<kore::Reg>(GET_REG1(instruction)),
                            static_cast<kore::Reg>(GET_REG2(instruction))
                        }
                    }
                };
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
            case kore::Bytecode::ArrayGet:
            case kore::Bytecode::ArraySet: {
                decoded_instruction = Instruction{
                    pos++,
                    byte_pos,
                    {
                        opcode,
                        kore::kir::ThreeRegisters{
                            static_cast<kore::Reg>(GET_REG1(instruction)),
                            static_cast<kore::Reg>(GET_REG2(instruction)),
                            static_cast<kore::Reg>(GET_REG3(instruction))
                        }
                    }
                };
                break;
            }

            case kore::Bytecode::Jump: {
                decoded_instruction = Instruction{
                    pos++,
                    byte_pos,
                    {
                        opcode,
                        kore::kir::Value{
                            static_cast<kore::Reg>(GET_OFFSET(instruction))
                        }
                    }
                };
                break;
            }

            case kore::Bytecode::Call: {
                int func_reg = GET_REG1(instruction);
                int arg_count = GET_REG2(instruction);
                int return_count = GET_REG3(instruction);
                int start_pos = pos++;
                int byte_offset = 0;

                auto arg_regs = decode_registers(pos, byte_offset, arg_count, obj);
                auto ret_regs = decode_registers(pos, byte_offset, return_count, obj);

                if (arg_count > 0 || return_count > 0) {
                    ++pos;
                }

                decoded_instruction = Instruction{
                    start_pos,
                    byte_pos,
                    { opcode, kore::kir::CallV{ func_reg, arg_regs, ret_regs } }
                };

                // Advance 4 positions for the instruction size + any additional
                // argument and return registers
                byte_pos += 4 + byte_offset;
                byte_pos_advanced = true;
                break;
            }

            case kore::Bytecode::Ret: {
                int regs = GET_REG1(instruction);
                int byte_offset = 2;
                auto ret_regs = decode_registers(pos, byte_offset, regs, obj);

                decoded_instruction = Instruction{
                    pos++,
                    byte_pos,
                    { opcode, kore::kir::ReturnV{ ret_regs } }
                };

                // Advance at least the instruction size if there were less than
                // 3 regsiters otherwise account for the additional return registers
                byte_pos += std::max(byte_offset, 4);
                byte_pos_advanced = true;
                break;
            }

            default:
                throw kore::ModuleLoadError("Failed to decode instruction", byte_pos);
        }

        if (!byte_pos_advanced) {
            byte_pos += 4;
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
