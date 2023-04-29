#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    namespace kir {
        Instruction::Instruction() {}

        Instruction::Instruction(InstructionType type)
            : _type(type) {}

        Instruction::Instruction(InstructionType type, Expression& expr)
            : _type(type), _expr(&expr) {}

        Instruction::Instruction(InstructionType type, kore::Reg reg1, Expression& expr)
                : Instruction(type, expr) {
            _registers.push_back(reg1);
        }

        Instruction::Instruction(InstructionType type, kore::Reg reg, const std::vector<Reg>& registers, Expression& expr)
            : _type(type), _expr(&expr), _registers(registers) {
            _registers.push_back(reg);
        }

        Instruction::Instruction(InstructionType type, kore::Reg reg1, kore::Reg reg2, Expression& expr)
                : Instruction(type, expr) {
            _registers.push_back(reg1);
            _registers.push_back(reg2);
        }

        Instruction::Instruction(InstructionType type, kore::Reg reg1, kore::Reg reg2, Statement& statement)
            : _type(type), _statement(&statement) {
            _registers.push_back(reg1);
            _registers.push_back(reg2);
        }

        Instruction::Instruction(
            InstructionType type,
            kore::Reg reg,
            BlockId bb1,
            BlockId bb2
        ) : _type(type) {
            _registers.push_back(reg);
            _bb1 = bb1;
            _bb2 = bb2;
        }

        Instruction::Instruction(Bytecode opcode) : _opcode(opcode) {
        }

        Instruction::Instruction(Bytecode opcode, kore::Reg reg) : Instruction(opcode) {
            _registers.push_back(reg);
        }

        Instruction::~Instruction() {}

        InstructionType Instruction::type() const noexcept {
            return _type;
        }

        kore::Reg Instruction::reg1() const {
            return _registers[0];
        }

        kore::Reg Instruction::reg2() const {
            return _registers[1];
        }

        kore::Reg Instruction::reg3() const {
            return _registers[2];
        }

        kore::Reg& Instruction::operator[](int index) {
            return _registers[index];
        }

        BlockId Instruction::bb1() {
            return _bb1;
        }

        BlockId Instruction::bb2() {
            return _bb2;
        }

        const Expression* Instruction::expr() const {
            return _expr;
        }

        const Statement* Instruction::statement() const {
            return _statement;
        }

        int Instruction::value() const noexcept {
            return _value;
        }

        std::vector<kore::Reg> Instruction::registers() const {
            return _registers;
        }

        std::ostream& operator<<(std::ostream& os, const Instruction instruction) {
            UNUSED_PARAM(instruction);
            os << "instruction";

            return os;
        }
    }
}
