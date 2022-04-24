#include <iostream>

#include "codegen/bytecode/bytecode.hpp"
#include "decode_instruction.hpp"

namespace kore {
    void decode_instructions(const CompiledObject* const compiled_object) {
        int pos = 0;

        std::cout << compiled_object->name() << std::endl;
        std::cout << compiled_object->location() << std::endl;
        std::cout << compiled_object->locals_count() << std::endl;
        std::cout << compiled_object->reg_count() << std::endl;

        for (auto instruction : *compiled_object) {
            auto opcode = static_cast<Bytecode>(instruction >> 24);

            switch (opcode) {
                case Bytecode::Noop:
                    std::cerr << pos << "     " << bytecode_to_string(opcode) << std::endl;
                    ++pos;
                    break;

                case Bytecode::LoadBool:
                case Bytecode::CloadI32:
                case Bytecode::CloadI64:
                case Bytecode::CloadF32:
                case Bytecode::CloadF64: {
                        Reg dest_reg = (instruction >> 16) & 0xff;
                        i32 value = instruction & 0xffff;

                        std::cerr << pos << "     "
                            << bytecode_to_string(opcode)
                            << " r" << dest_reg << " "
                            << value << std::endl;

                        ++pos;
                    }
                    break;

                case Bytecode::Move:
                case Bytecode::StoreI32Global: {
                        Reg dest_reg = (instruction >> 16) & 0xff;
                        Reg reg = (instruction >> 8) & 0xff;

                        std::cerr << pos << "     "
                            << bytecode_to_string(opcode)
                            << " r" << dest_reg
                            << " r" << reg << std::endl;

                        ++pos;
                    }
                    break;

                case Bytecode::AddI32:
                case Bytecode::AddI64:
                case Bytecode::AddF32:
                case Bytecode::AddF64:
                case Bytecode::SubI32:
                case Bytecode::SubI64:
                case Bytecode::SubF32:
                case Bytecode::SubF64:
                case Bytecode::MultI32:
                case Bytecode::MultI64:
                case Bytecode::MultF32:
                case Bytecode::MultF64:
                case Bytecode::PowI32:
                case Bytecode::PowI64:
                case Bytecode::PowF32:
                case Bytecode::PowF64:
                case Bytecode::DivI32:
                case Bytecode::DivI64:
                case Bytecode::DivF32:
                case Bytecode::DivF64:
                case Bytecode::LtI32:
                case Bytecode::LtI64:
                case Bytecode::LtF32:
                case Bytecode::LtF64:
                case Bytecode::GtI32:
                case Bytecode::GtI64:
                case Bytecode::GtF32:
                case Bytecode::GtF64:
                case Bytecode::LeI32:
                case Bytecode::LeI64:
                case Bytecode::LeF32:
                case Bytecode::LeF64:
                case Bytecode::GeI32:
                case Bytecode::GeI64:
                case Bytecode::GeF32:
                case Bytecode::GeF64:
                case Bytecode::EqI32:
                case Bytecode::EqI64:
                case Bytecode::EqF32:
                case Bytecode::EqF64:
                case Bytecode::NeqI32:
                case Bytecode::NeqI64:
                case Bytecode::NeqF32:
                case Bytecode::NeqF64: {
                        Reg dest_reg = (instruction >> 16) & 0xff;
                        Reg reg_operand1 = (instruction >> 8) & 0xff;
                        Reg reg_operand2 = instruction & 0xff;

                        std::cerr << pos << "     "
                            << bytecode_to_string(opcode) << " "
                            << "r" << dest_reg << " "
                            << "r" << reg_operand1 << " "
                            << "r" << reg_operand2 << std::endl;
                        ++pos;
                    }
                    break;

                case Bytecode::Jump: {
                        auto offset = instruction & 0xffff;

                        std::cerr << pos
                            << "    "
                            << bytecode_to_string(opcode)
                            << " "
                            << offset
                            << std::endl;

                        ++pos;
                    }
                    break;

                case Bytecode::JumpIf:
                case Bytecode::JumpIfNot: {
                        Reg reg = (instruction >> 16) & 0xff;
                        auto offset = instruction & 0xffff;

                        std::cerr << pos
                            << "     "
                            << bytecode_to_string(opcode)
                            << " r" << reg
                            << " "
                            << offset
                            << std::endl;

                        ++pos;
                    }
                    break;

                case Bytecode::Ret: {
                        std::cerr << pos
                            << "     "
                            << bytecode_to_string(opcode)
                            << std::endl;

                        ++pos;
                    }
                    break;

                case Bytecode::RetReg: {
                        Reg reg = (instruction >> 16) & 0xff;

                        std::cerr << pos
                            << "     "
                            << bytecode_to_string(opcode)
                            << " r" << reg
                            << std::endl;

                        ++pos;
                    }
                    break;

                default:
                    std::cerr
                        << "Opcode "
                        << bytecode_to_string(opcode)
                        << " not yet implemented"
                        << std::endl;
                    ++pos;
                    break;
            }
        }
    }
}
