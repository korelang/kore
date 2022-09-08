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
        int ret_reg,
        const std::vector<kore::Reg>& call_registers
    ) {
        auto instruction = Instruction(opcode, pos, ret_reg);
        instruction._call_registers = call_registers;

        return instruction;
    }

    int Instruction::position() const {
        return _pos;
    }

    kore::Bytecode Instruction::opcode() const {
        return _opcode;
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

    std::vector<kore::Reg> Instruction::call_registers() const {
        return _call_registers;
    }

    std::ostream& operator<<(std::ostream& os, const Instruction instruction) {
        os << instruction.position() << ": " << bytecode_to_string(instruction.opcode());

        switch (instruction.opcode()) {
            case kore::Bytecode::CloadI32:
            case kore::Bytecode::CloadI64:
            case kore::Bytecode::CloadF32:
            case kore::Bytecode::CloadF64:
                os << " @" << instruction.reg1() << " #" << instruction.value();
                return os;

            case kore::Bytecode::Call:
                os << " @" << instruction.reg1();

                for (auto call_register : instruction.call_registers()) {
                    os << " @" << call_register;
                }

                return os;

            default:
                break;
        }

        if (instruction.reg1() >= 0) {
            os << " @" << instruction.reg1();
        }


        if (instruction.reg2() >= 0) {
            os << " @" << instruction.reg2();
        }

        if (instruction.reg3() >= 0) {
            os << " @" << instruction.reg3();
        }

        return os;
    }
}
