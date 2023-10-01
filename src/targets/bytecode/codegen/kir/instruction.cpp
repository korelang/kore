#include "targets/bytecode/codegen/kir/instruction.hpp"
#include <ios>
#include "utils/unused_parameter.hpp"

namespace kore {
    namespace kir {
        Instruction::Instruction() {}

        Instruction::Instruction(InstructionType type)
            : _type(type) {}

        Instruction::Instruction(InstructionType type, Expression& expr)
            : _type(type), _expr(&expr) {}

        Instruction::Instruction(InstructionType type, kore::Reg reg1, Expression& expr, int value)
                : Instruction(type, expr) {
            _value = value;
            _registers.push_back(reg1);
        }

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

        Instruction::Instruction(InstructionType type, kore::Reg reg1, kore::Reg reg2, kore::Reg reg3, Expression& expr)
                : Instruction(type, expr) {
            _registers.push_back(reg1);
            _registers.push_back(reg2);
            _registers.push_back(reg3);
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

        Instruction::Instruction(Bytecode opcode)
            : _type(InstructionType::Raw), _opcode(opcode) {
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

        BlockId Instruction::bb1() const {
            return _bb1;
        }

        BlockId Instruction::bb2() const {
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

        Bytecode Instruction::opcode() const {
            return _opcode;
        }

        std::vector<kore::Reg> Instruction::registers() const {
            return _registers;
        }

        std::ostream& operator<<(std::ostream& os, const Instruction instruction) {
            switch (instruction.type()) {
                case kir::InstructionType::LoadBool: {
                    bool value = instruction.expr_as<BoolExpression>()->bool_value();

                    os << bytecode_to_string(Bytecode::LoadBool) << " "
                       << instruction.reg1() << " "
                       << std::boolalpha << value << std::noboolalpha;
                    break;
                }

                case kir::InstructionType::LoadInteger: {
                    auto expr = instruction.expr_as<IntegerExpression>();

                    os << bytecode_to_string(Bytecode::CloadI32) << " "
                       << instruction.reg1() << " "
                       << expr->value();
                    break;
                }

                case kir::InstructionType::LoadGlobal: {
                    os << bytecode_to_string(Bytecode::Gload) << " "
                       << instruction.reg1() << " "
                       << instruction.value();
                    break;
                }

                case kir::InstructionType::Move: {
                    os << bytecode_to_string(Bytecode::Move) << " "
                       << instruction.reg1() << " "
                       << instruction.reg2();
                    break;
                }

                case kir::InstructionType::Binop: {
                    /* auto binexpr = instruction.expr_as<BinaryExpression>(); */
                    /* auto opcode = get_binop_instruction( */
                    /*     binexpr->type()->category(), */
                    /*     binexpr->op() */
                    /* ); */

                    // TODO
                    os << bytecode_to_string(Bytecode::AddI32) << " "
                       << instruction.reg1() << " "
                       << instruction.reg2() << " "
                       << instruction.reg3();
                    break;
                }

                case kir::InstructionType::Branch: {
                    // When dumping kir, jump offsets are still basic block IDs
                    // (they get patched to instruction offsets in the code
                    // generator)
                    os << bytecode_to_string(Bytecode::JumpIfNot) << " "
                       << instruction.reg1() << " "
                       << "bb" << instruction.bb1() << " "
                       << "bb" << instruction.bb2();
                    break;
                }

                case kir::InstructionType::AllocateArray: {
                    os << bytecode_to_string(Bytecode::AllocArray) << " "
                       << instruction.reg1() << " "
                       << instruction.value();
                    break;
                }

                case kir::InstructionType::BuiltinFunctionCall: {
                    os << bytecode_to_string(Bytecode::BuiltinCall) << " "
                       << instruction.reg1() << " "
                       << instruction.reg2() << " "
                       << instruction.reg3();
                    break;
                }

                case kir::InstructionType::Call: {
                    os << bytecode_to_string(Bytecode::Call) << " "
                       << instruction.reg1() << " "
                       << instruction.reg2() << " "
                       << instruction.reg3();
                    break;
                }

                case kir::InstructionType::LoadFloat: {
                    break;
                }

                case kir::InstructionType::Raw: {
                    os << bytecode_to_string(instruction.opcode());
                    auto registers = instruction.registers();

                    for (size_t i = 0; i < registers.size(); ++i) {
                        os << " " << registers[i];
                    }
                    break;
                }
            }

            return os;
        }
    }
}
