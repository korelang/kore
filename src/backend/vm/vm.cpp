#include <sstream>

#include "backend/vm/vm.hpp"

#define LOAD_OPCODE(type) {\
    Reg dest_reg;\
    Reg value;\
    decode_address2_opcode(curr_instruction, &dest_reg, &value);\
    _registers[dest_reg] = Value::from_##type(value);\
}

#define BINARY_ARITH_OPCODE(type, op) {\
    Reg dest_reg;\
    Reg op1_reg, op2_reg;\
    decode_address3_opcode(curr_instruction, &dest_reg, &op1_reg, &op2_reg);\
    \
    auto value1 = _registers[op1_reg].as_##type();\
    auto value2 = _registers[op2_reg].as_##type();\
    \
    _registers[dest_reg] = Value::from_##type(value1 op value2);\
}

#define BINARY_OP_CASES(type, opcode_suffix) {\
    case Bytecode::Add##opcode_suffix:\
        BINARY_ARITH_OPCODE(type, +)\
        break;\
    \
    case Bytecode::Sub##opcode_suffix:\
        BINARY_ARITH_OPCODE(type, -)\
        break;\
    \
    case Bytecode::Mult##opcode_suffix:\
        BINARY_ARITH_OPCODE(type, *)\
        break;\
    \
    case Bytecode::Div##opcode_suffix:\
        BINARY_ARITH_OPCODE(type, /)\
        break;\
}

#define RELOP_OPCODE(type, op) {\
    Reg dest_reg;\
    Reg op1_reg, op2_reg;\
    decode_address3_opcode(curr_instruction, &dest_reg, &op1_reg, &op2_reg);\
    \
    auto value1 = _registers[op1_reg].as_##type();\
    auto value2 = _registers[op2_reg].as_##type();\
    \
    _registers[dest_reg] = Value::from_bool(value1 op value2);\
}

#define RELOP_CASES(type, opcode_suffix) {\
    case Bytecode::Lt##opcode_suffix:\
        RELOP_OPCODE(type, <)\
        break;\
    \
    case Bytecode::Gt##opcode_suffix:\
        RELOP_OPCODE(type, <)\
        break;\
    \
    case Bytecode::Le##opcode_suffix:\
        RELOP_OPCODE(type, <)\
        break;\
    \
    case Bytecode::Ge##opcode_suffix:\
        RELOP_OPCODE(type, <)\
        break;\
    \
    case Bytecode::Eq##opcode_suffix:\
        RELOP_OPCODE(type, <)\
        break;\
    \
    case Bytecode::Neq##opcode_suffix:\
        RELOP_OPCODE(type, <)\
        break;\
}

namespace kore {
    namespace vm {
        Vm::Vm() {}

        Vm::~Vm() {}

        void Vm::run(const bytecode_type* code, std::size_t size) {
            if (!code || size < 1) {
                return;
            }

            _sp = 0;
            _fp = 0;

            // Main interpreter dispatch loop
            while (_pc < size) {
                auto curr_instruction = code[_pc++];
                auto opcode = decode_opcode(curr_instruction);

                switch (opcode) {
                    case Bytecode::CloadI32:
                        LOAD_OPCODE(i32);
                        break;

                    case Bytecode::CloadI64:
                        LOAD_OPCODE(i64);
                        break;

                    BINARY_OP_CASES(i32, I32)
                    BINARY_OP_CASES(i64, I64)
                    BINARY_OP_CASES(f32, F32)
                    BINARY_OP_CASES(f64, F64)

                    RELOP_CASES(i32, I32)
                    RELOP_CASES(i64, I64)
                    RELOP_CASES(f32, F32)
                    RELOP_CASES(f64, F64)

                    case Bytecode::Move: {
                        _registers[GET_REG1(curr_instruction)] = _registers[GET_REG2(curr_instruction)];
                        break;
                    }

                    case Bytecode::Jump: {
                        _pc += GET_OFFSET(curr_instruction);
                        break;
                    }

                    case Bytecode::JumpIf: {
                        if (_registers[GET_REG1(curr_instruction)].as_bool()) {
                            // Adjust since we already incremented the pc
                            _pc += GET_OFFSET(curr_instruction) - 1;
                        }
                        break;
                    }

                    case Bytecode::JumpIfNot: {
                        if (!_registers[GET_REG1(curr_instruction)].as_bool()) {
                            // Adjust since we already incremented the pc
                            _pc += GET_OFFSET(curr_instruction) - 1;
                        }
                        break;
                    }

                    case Bytecode::GstoreI32:
                        break;

                    /* case Bytecode::Call: { */
                    /*     push(_context.fp); */
                    /*     push(_context.pc); */
                    /* } */
                    /* break; */

                    default:
                        throw_unknown_opcode(opcode);
                }
            }
        }

        void Vm::run(const std::vector<bytecode_type>& code) {
            run(code.data(), code.size());
        }

        /* void Vm::run_module(const Module& module) { */
        /*     add_module(module); */
        /*     context.set_from_module(module); */
        /* } */

        inline Bytecode Vm::decode_opcode(bytecode_type instruction) {
            return static_cast<Bytecode>(instruction >> 24);
        }

        void inline Vm::decode_address2_opcode(bytecode_type opcode, Reg* dest_reg, Reg* value) {
            *dest_reg = (opcode >> 16) & 0xff;
            *value = opcode & 0xffff;
        }

        void inline Vm::decode_address3_opcode(bytecode_type opcode, Reg* dest_reg, Reg* op1, Reg* op2) {
            *dest_reg = (opcode >> 16) & 0xff;
            *op1 = (opcode >> 8) & 0xff;
            *op2 = opcode & 0xff;
        }

        void Vm::throw_unknown_opcode(Bytecode opcode) {
            std::ostringstream oss;
            oss << "Unsupported bytecode at " << _pc << ": " << opcode;
            throw std::runtime_error(oss.str());
        }
    }
}

#undef LOAD_OPCODE
#undef BINARY_ARITH_OPCODE
#undef BINARY_OP_CASES
#undef RELOP_OPCODE
#undef RELOP_CASES
