// const bool isCpuLittleEndian = 1 == *(char*)(&__one__); // CPU endianness

#include <sstream>
#include <iostream>

#include "backend/vm/vm.hpp"

#define LOAD_OPCODE(type) {\
    int dest_reg;\
    int value;\
    decode_address2_opcode(curr_instruction, &dest_reg, &value);\
    _registers[dest_reg] = from_##type(value);\
}

#define BINARY_ARITH_OPCODE(type) {\
    int dest_reg;\
    int op1_reg, op2_reg;\
    decode_address3_opcode(curr_instruction, &dest_reg, &op1_reg, &op2_reg);\
    \
    auto value1 = _registers[op1_reg].as_##type();\
    auto value2 = _registers[op2_reg].as_##type();\
    \
    _registers[dest_reg] = from_##type(value1 + value2);\
}

namespace kore {
    Vm::Vm() {}

    Vm::~Vm() {}

    void Vm::run(const bytecode_type* code, std::size_t size) {
        if (!code || size < 1) {
        }

        _pc = 0;

        // Main interpreter dispatch loop
        while (_pc < size) {
            auto curr_instruction = code[_pc++];
            auto opcode = decode_opcode(curr_instruction);

            switch (opcode) {
                case Bytecode::LoadI32:
                    LOAD_OPCODE(i32)
                    break;

                case Bytecode::LoadI64:
                    LOAD_OPCODE(i64)
                    break;

                case Bytecode::AddI32:
                    BINARY_ARITH_OPCODE(i32)
                    break;

                case Bytecode::AddI64:
                    BINARY_ARITH_OPCODE(i64)
                    break;

                case Bytecode::AddF32:
                    BINARY_ARITH_OPCODE(f32)
                    break;

                case Bytecode::AddF64:
                    BINARY_ARITH_OPCODE(f64)
                    break;

                case Bytecode::StoreI32Global:
                    break;

                default: {
                    std::ostringstream oss;
                    oss << "Unsupported bytecode at " << _pc << ": " << opcode;
                    throw std::runtime_error(oss.str());
                }
            }
        }

        /* #ifdef KORE_VM_DEBUG */
        for (int i = 0; i < 3/*KORE_VM_MAX_REGISTERS*/; ++i) {
            std::cerr << "r" << i << " => " << _registers[i].as_i32() << std::endl;
        }
        /* #endif */
    }

    void Vm::run(const std::vector<bytecode_type>& code) {
        run(code.data(), code.size());
    }

    inline Bytecode Vm::decode_opcode(bytecode_type instruction) {
        return static_cast<Bytecode>(instruction >> 24);
    }

    void inline Vm::decode_address2_opcode(bytecode_type opcode, int* dest_reg, int* value) {
        *dest_reg = (opcode >> 16) & 0xff;
        *value = opcode & 0xffff;
    }

    void inline Vm::decode_address3_opcode(bytecode_type opcode, int* dest_reg, int* op1, int* op2) {
        *dest_reg = (opcode >> 16) & 0xff;
        *op1 = (opcode >> 8) & 0xff;
        *op2 = opcode & 0xff;
    }
}

#undef LOAD_OPCODE
#undef BINARY_ARITH_OPCODE
