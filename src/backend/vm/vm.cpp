#include "module_loader.hpp"
#include "utils/unused_parameter.hpp"
#include "vm/vm.hpp"

#define LOAD_OPCODE(type) {\
    Reg dest_reg = GET_REG1(opcode);\
    Reg value = GET_VALUE(opcode);\
    _registers[dest_reg] = Value::from_##type(value);\
}

#define BINARY_OP(arg_type, ret_type, op) {\
    Reg dest_reg = GET_REG1(instruction);\
    Reg op1_reg = GET_REG2(instruction), op2_reg = GET_REG3(instruction);\
    \
    auto value1 = _registers[fp + op1_reg].as_##arg_type();\
    auto value2 = _registers[fp + op2_reg].as_##arg_type();\
    \
    _registers[fp + dest_reg] = Value::from_##ret_type(value1 op value2);\
}

#define BINARY_OP_CASES(type, opcode_suffix) {\
    case Bytecode::Add##opcode_suffix:\
        BINARY_OP(type, type, +)\
        break;\
    \
    case Bytecode::Sub##opcode_suffix:\
        BINARY_OP(type, type, +)\
        break;\
    \
    case Bytecode::Mult##opcode_suffix:\
        BINARY_OP(type, type, +)\
        break;\
    \
    case Bytecode::Div##opcode_suffix:\
        BINARY_OP(type, type, +)\
        break;\
}

#define RELOP_CASES(type, opcode_suffix) {\
    case Bytecode::Lt##opcode_suffix:\
        BINARY_OP(type, bool, +)\
        break;\
    \
    case Bytecode::Gt##opcode_suffix:\
        BINARY_OP(type, bool, <)\
        break;\
    \
    case Bytecode::Le##opcode_suffix:\
        BINARY_OP(type, bool, <)\
        break;\
    \
    case Bytecode::Ge##opcode_suffix:\
        BINARY_OP(type, bool, <)\
        break;\
    \
    case Bytecode::Eq##opcode_suffix:\
        BINARY_OP(type, bool, <)\
        break;\
    \
    case Bytecode::Neq##opcode_suffix:\
        BINARY_OP(type, bool, <)\
        break;\
}

namespace kore {
    namespace vm {
        void Context::reset() {
            pc = 0, sp = 0, fp = 0;
        }

        void Context::restore(int registers, std::size_t fp, std::size_t pc) {
            this->sp -= registers + 2;
            this->fp = fp;
            this->pc = pc;
        }

        Vm::Vm() {}

        Vm::~Vm() {}

        void Vm::run(const bytecode_type* code, std::size_t size) {
            if (!code || size < 1) {
                return;
            }

            _context.reset();

            // Push a call frame for the main object
            _call_frames.push_back(CallFrame{ 0, 0, code, size });

            // Main interpreter dispatch loop
            while (_context.pc < size) {
                auto instruction = current_frame().code[_context.pc++];
                auto opcode = GET_OPCODE(instruction);
                auto fp = _context.fp;

                switch (opcode) {
                    case Bytecode::Noop:
                        break;

                    case Bytecode::Move: {
                        _registers[fp + GET_REG1(instruction)] = _registers[fp + GET_REG2(instruction)];
                        break;
                    }

                    case Bytecode::LoadBool: {
                        Reg dest_reg = GET_REG1(opcode);
                        _registers[fp + dest_reg] = Value::from_bool(GET_VALUE(opcode));
                        break;
                    }

                    // TODO: Get constants from current module
                    case Bytecode::CloadI32:
                        LOAD_OPCODE(i32);
                        break;

                    case Bytecode::CloadI64:
                        LOAD_OPCODE(i64);
                        break;

                    case Bytecode::Gload: {
                        Reg dest_reg = GET_REG1(opcode);
                        _registers[fp + dest_reg] = _registers[GET_VALUE(opcode)];
                        break;
                    }

                    BINARY_OP_CASES(i32, I32)
                    BINARY_OP_CASES(i64, I64)
                    BINARY_OP_CASES(f32, F32)
                    BINARY_OP_CASES(f64, F64)

                    RELOP_CASES(i32, I32)
                    RELOP_CASES(i64, I64)
                    RELOP_CASES(f32, F32)
                    RELOP_CASES(f64, F64)

                    case Bytecode::Jump: {
                        _context.pc += GET_OFFSET(instruction);
                        break;
                    }

                    case Bytecode::JumpIf: {
                        if (_registers[fp + GET_REG1(instruction)].as_bool()) {
                            // Adjust since we already incremented the pc
                            _context.pc += GET_OFFSET(instruction) - 1;
                        }
                        break;
                    }

                    case Bytecode::JumpIfNot: {
                        if (!_registers[fp + GET_REG1(instruction)].as_bool()) {
                            // Adjust since we already incremented the pc
                            _context.pc += GET_OFFSET(instruction) - 1;
                        }
                        break;
                    }

                    case Bytecode::Call: {
                        push_call_frame(instruction);
                        break;
                    }

                    case kore::Bytecode::Ret: {
                        pop_call_frame(instruction);
                        break;
                    }

                    default:
                        throw_unknown_opcode(opcode);
                }
            }
        }

        void Vm::run(const std::vector<bytecode_type>& code) {
            run(code.data(), code.size());
        }

        /* void Vm::run_path(const fs::path& path) { */
        /*     auto module = load_module_from_path(path); */
        /*     run_module(module); */
        /* } */

        /* void Vm::run_module(Module& module) { */
        /*     auto main_object = module.main_object(); */

        /*     run(main_object->instructions(), main_object->code_size()); */
        /* } */

        void Vm::throw_unknown_opcode(Bytecode opcode) {
            auto message = "Unsupported bytecode at "  + std::to_string(_context.pc) + ": " + std::to_string(opcode);

            throw std::runtime_error(message);
        }

        int Vm::get_function(int func_index) {
            UNUSED_PARAM(func_index);

            // TODO
            return 0;
        }

        CallFrame& Vm::current_frame() {
            return _call_frames.back();
        }

        void Vm::push_i32(i32 value) {
            _registers[_context.sp++] = Value::from_i32(value);
        }

        void Vm::push(Value& value) {
            _registers[_context.sp++] = value;
        }

        void Vm::push_register(Reg reg) {
            push(_registers[reg]);
        }

        void Vm::push_call_frame(bytecode_type instruction) {
            // Push the current frame pointer and program counter
            push_i32(_context.fp);
            push_i32(_context.pc);

            // Save the current stack pointer position as the new frame pointer
            _context.fp = _context.sp;

            // Get the number of return and argument registers
            int func_index = GET_REG1(instruction);
            int ret_count = GET_REG2(instruction);
            int arg_count = GET_REG3(instruction);
            CallFrame& frame = current_frame();

            // Push argument registers onto the call stack
            for (int i = 0, j = 24; i < arg_count; ++i, j -= 8) {
                Reg arg_reg = (instruction >> j) & 0xff;
                push_register(arg_reg);

                if (j == 0) {
                    j = 24;
                    instruction = frame.code[_context.pc++];
                }
            }

            auto function = get_function(func_index);
            UNUSED_PARAM(function);

            _call_frames.push_back(CallFrame{ ret_count, 0, nullptr, 0 });
        }

        Value Vm::pop() {
            return _registers[--_context.sp];
        }

        void Vm::pop_call_frame(bytecode_type instruction) {
            UNUSED_PARAM(instruction);

            // Get the old frame pointer and program counter
            int old_fp = _registers[_context.fp - 2].as_i32();
            int old_pc = _registers[_context.fp - 1].as_i32();

            CallFrame& frame = current_frame();

            // How many values are we returning?
            int ret_count = frame.ret_count;

            // Copy return registers into destination registers in the previous
            // call frame
            for (int i = 0; i < ret_count; ++i) {
                // TODO: Fix
                move(_context.fp + i, top() - i);
            }

            // Reset the stack pointer to the base of the current call frame
            // and beyond the old frame pointer and program counter. Restore
            // the old frame pointer and program counter
            _context.restore(frame.reg_count, old_fp, old_pc);

            _call_frames.pop_back();
        }

        inline int Vm::top() {
            return _context.sp - 1;
        }

        void Vm::move(Reg src_reg, Reg dst_reg) {
            _registers[dst_reg] = _registers[src_reg];
        }

        void Vm::add_module(Module& module) {
            _modules.try_emplace(module.name(), std::move(module));
        }
    }
}

#undef LOAD_OPCODE
#undef BINARY_ARITH_OPCODE
#undef BINARY_OP_CASES
#undef RELOP_OPCODE
#undef RELOP_CASES
