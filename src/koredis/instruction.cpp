#include <sstream>
#include "instruction.hpp"

namespace koredis {
    Instruction::Instruction()
        : Instruction(kore::Bytecode::Noop, -1, -1, -1, -1) {}

    Instruction::Instruction(kore::Bytecode opcode, int pos)
        : Instruction(opcode, pos, -1, -1, -1) {}

    Instruction::Instruction(kore::Bytecode opcode, int pos, kore::Reg reg1)
        : Instruction(opcode, pos, reg1, -1, -1) {}

    Instruction::Instruction(kore::Bytecode opcode, int pos, kore::Reg reg1, kore::Reg reg2)
        : Instruction(opcode, pos, reg1, reg2, -1) {}

    Instruction::Instruction(kore::Bytecode opcode, int pos, kore::Reg reg1, kore::Reg reg2, kore::Reg reg3)
        : _pos(pos),
          _opcode(opcode),
          _reg1(reg1),
          _reg2(reg2),
          _reg3(reg3) {}

    Instruction::~Instruction() {}

    Instruction Instruction::load(kore::Bytecode opcode, int pos, int reg1, int value) {
        return Instruction::with_offset(opcode, pos, reg1, value);
    }

    Instruction Instruction::with_offset(kore::Bytecode opcode, int pos, int reg1, int offset) {
        auto instruction = Instruction(opcode, pos, reg1);
        instruction._value = offset;

        return instruction;
    }

    Instruction Instruction::call(
        kore::Bytecode opcode,
        int pos,
        int func_index,
        int return_count,
        int arg_count,
        const std::vector<kore::Reg>& return_registers,
        const std::vector<kore::Reg>& arg_registers
    ) {
        auto instruction = Instruction(opcode, pos, func_index, return_count, arg_count);
        instruction._return_registers = return_registers;
        instruction._arg_registers = arg_registers;

        return instruction;
    }

    Instruction Instruction::ret(
        kore::Bytecode opcode,
        int pos,
        int return_count,
        const std::vector<kore::Reg>& return_registers
    ) {
        auto instruction = Instruction(opcode, pos, return_count);
        instruction._return_registers = return_registers;

        return instruction;
    }

    int Instruction::position() const {
        return _pos;
    }

    kore::Bytecode Instruction::opcode() const {
        return _opcode;
    }

    std::string Instruction::name() const {
        return bytecode_to_string(opcode());
    }

    kore::Reg Instruction::reg1() const {
        return _reg1;
    }

    kore::Reg Instruction::reg2() const {
        return _reg2;
    }

    kore::Reg Instruction::reg3() const {
        return _reg3;
    }

    int Instruction::value() const {
        return _value;
    }

    std::vector<kore::Reg> Instruction::return_registers() const {
        return _return_registers;
    }

    std::vector<kore::Reg> Instruction::call_registers() const {
        return _arg_registers;
    }

    std::string Instruction::registers_as_string() const {
        std::ostringstream oss;

        // Special-case output for more complex opcodes
        switch (opcode()) {
            case kore::Bytecode::LoadBool: {
                oss << " @" << reg1() << " " << (value() == 1 ? "true" : "false");
                return oss.str();
            }

            case kore::Bytecode::Jump: {
                oss << value();
                return oss.str();
            }

            case kore::Bytecode::JumpIf:
            case kore::Bytecode::JumpIfNot: {
                oss << " @" << reg1() << " " << value();
                return oss.str();
            }

            case kore::Bytecode::CloadI32:
            case kore::Bytecode::CloadI64:
            case kore::Bytecode::CloadF32:
            case kore::Bytecode::CloadF64:
            case kore::Bytecode::Gload:
                oss << " @" << reg1() << " #" << value();
                return oss.str();

            case kore::Bytecode::Call: {
                oss << " @" << reg1() << " " << reg2() << " " << reg3();

                for (auto call_register : call_registers()) {
                    oss << " @" << call_register;
                }

                for (auto ret_register : return_registers()) {
                    oss << " @" << ret_register;
                }

                return oss.str();
            }

            case kore::Bytecode::Ret: {
                auto ret_count = reg1();

                if (ret_count > 0) {
                    oss << ret_count << " @" << reg2();
                }

                return oss.str();
            }

            case kore::Bytecode::Gstore: {
                oss << "#" << reg1() << " @" << reg2();
                return oss.str();
            }

            default:
                break;
        }

        if (reg1() >= 0) {
            oss << " @" << reg1();
        }


        if (reg2() >= 0) {
            oss << " @" << reg2();
        }

        if (reg3() >= 0) {
            oss << " @" << reg3();
        }

        return oss.str();
    }

    std::ostream& operator<<(std::ostream& os, const Instruction instruction) {
        os << instruction.position() << ": " << instruction.name() << instruction.registers_as_string();

        return os;
    }
}
