#include "targets/bytecode/module_loader.hpp"
#include "targets/bytecode/vm/config.hpp"
#include "targets/bytecode/vm/vm.hpp"
#include "utils/unused_parameter.hpp"

#define LOAD_OPCODE(type) {\
    Reg dest_reg = GET_REG1(instruction);\
    int index = GET_VALUE(instruction);\
    \
    _registers[fp + dest_reg] = Value::from_##type(\
        _context._current_module->type##_constant_table().get(index)\
    );\
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
        BINARY_OP(type, type, -)\
        break;\
    \
    case Bytecode::Mult##opcode_suffix:\
        BINARY_OP(type, type, *)\
        break;\
    \
    case Bytecode::Div##opcode_suffix:\
        BINARY_OP(type, type, /)\
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

#define _KORE_DEBUG_VM defined(KORE_DEBUG_VM) || defined(KORE_DEBUG)

#ifdef _KORE_DEBUG_VM
    #include "logging/logging.hpp"

    #define KORE_DEBUG_VM_LOG(action, value) {\
        if (!value.empty()) {\
            debug_group("vm", "%s: %s", action, value.c_str());\
        } else {\
            debug_group("vm", action);\
        }\
    }
#else
    #define KORE_DEBUG_VM_LOG(action, value)
#endif

namespace kore {
    namespace vm {
        void Context::reset() {
            pc = 0, sp = 0, fp = 0;
        }

        void Context::restore(const CallFrame& call_frame) {
            this->sp -= call_frame.reg_count;
            this->fp = call_frame.old_fp;
            this->pc = call_frame.old_pc;
        }

        CallFrame::CallFrame(int ret_count, int reg_count, const bytecode_type* code, std::size_t size)
            : ret_count(ret_count),
              reg_count(reg_count),
              code(code),
              size(size) {}

        CallFrame::CallFrame(int ret_count, int shift, std::size_t old_fp, std::size_t old_pc, CompiledObject* obj)
            : ret_count(ret_count),
              reg_count(obj->reg_count()),
              shift(shift),
              old_fp(old_fp),
              old_pc(old_pc),
              code(obj->instructions()),
              size(obj->code_size()) {}

        Vm::Vm() {}

        Vm::~Vm() {}

        void Vm::run(const bytecode_type* code, std::size_t size) {
            if (!code || size < 1) {
                return;
            }

            _context.reset();
            main_call_frame_reg_count = current_frame().reg_count;
            _context.sp += main_call_frame_reg_count;

            // Main interpreter dispatch loop
            while (_call_frames.size() > 0) {//_context.pc < size) {
                auto instruction = current_frame().code[_context.pc++];
                auto opcode = GET_OPCODE(instruction);
                auto fp = _context.fp;

                KORE_DEBUG_VM_LOG("dispatch", bytecode_to_string(opcode));

                switch (opcode) {
                    case Bytecode::Noop:
                        break;

                    case Bytecode::Move: {
                        move(fp + GET_REG1(instruction), fp + GET_REG2(instruction));
                        break;
                    }

                    case Bytecode::LoadBool: {
                        Reg dest_reg = GET_REG1(instruction);
                        _registers[fp + dest_reg] = Value::from_bool(GET_VALUE(instruction));
                        break;
                    }

                    case Bytecode::CloadI32: {
                        LOAD_OPCODE(i32);
                        break;
                    }

                    case Bytecode::CloadI64: {
                        LOAD_OPCODE(i64);
                        break;
                    }

                    case Bytecode::Gload: {
                        Reg dest_reg = GET_REG1(instruction);
                        _registers[fp + dest_reg] = _globals[GET_REG2(instruction)];
                        break;
                    }

                    case Bytecode::Gstore: {
                        Reg dest_reg = GET_REG1(instruction);
                        _globals[dest_reg] = _registers[fp + GET_REG2(instruction)];
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
                            // Adjust by -1 since we already incremented the pc
                            _context.pc += GET_OFFSET(instruction) - 1;
                        }
                        break;
                    }

                    case Bytecode::JumpIfNot: {
                        if (!_registers[fp + GET_REG1(instruction)].as_bool()) {
                            // Adjust by -1 since we already incremented the pc
                            _context.pc += GET_OFFSET(instruction) - 1;
                        }
                        break;
                    }

                    case Bytecode::Call: {
                        do_function_call(instruction);
                        break;
                    }

                    case Bytecode::Ret: {
                        do_function_return(instruction);
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

        void Vm::dump_registers(std::ostream& os) {
            for (int i = 0; i < main_call_frame_reg_count; ++i) {
                os << "@" << i << std::setw(6) << std::left << " " << _registers[i] << std::endl;
            }
        }

        void Vm::run_path(const fs::path& path) {
            KORE_DEBUG_VM_LOG("run path", path);
            auto module = kore::load_module_from_path(path);

            run_module(module);
        }

        void Vm::run_module(Module& module) {
            // Allocate space for globals
            allocate_globals(module);

            add_module(module);

            // Push a call frame to the main object
            auto main_object = _context._current_module->main_object();
            _call_frames.push_back(CallFrame{ 0, 0, 0, 0, main_object });

            run_compiled_object(main_object);
        }

        void Vm::run_compiled_object(CompiledObject* obj) {
            run(obj->instructions(), obj->code_size());
        }

        void Vm::load_functions_from_module(const Module& module) {
            KORE_DEBUG_VM_LOG("module load", module.path());
            _loaded_functions.resize(_loaded_functions.size() + module.objects_count());

            for (auto it = module.objects_begin(); it != module.objects_end(); ++it) {
                _loaded_functions[(*it)->func_index()] = it->get();
            }
        }

        void Vm::allocate_globals(const Module& module) {
            _globals.resize(_globals.size() + module.global_indices_count());
        }

        void Vm::allocate_local_stack(const CompiledObject* const obj) {
            auto reg_count = obj->reg_count();

            if (_context.sp + reg_count >= KORE_VM_MAX_REGISTERS) {
                throw_vm_error("Stack-overflow, ran out of local stack");
            }

            _context.sp += reg_count;
        }

        void Vm::throw_vm_error(const std::string& message) {
            throw std::runtime_error(message);
        }

        void Vm::throw_unknown_opcode(Bytecode opcode) {
            auto message = "Unsupported bytecode at "  + std::to_string(_context.pc) + ": " + std::to_string(opcode);

            throw_vm_error(message);
        }

        CompiledObject* Vm::get_function(int func_index) {
            return _loaded_functions[func_index];
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

        const CallFrame& Vm::get_caller() {
            #ifdef _KORE_DEBUG_VM
                if (_call_frames.size() < 2) {
                    throw std::runtime_error("no previous caller");
                }
            #endif

            return _call_frames[_call_frames.size() - 2];
        }

        /* void Vm::push_call_frame(int ret_count, int shift, int old_fp, CompiledObject* function) { */
        /*     _call_frames.push_back(CallFrame{ ret_count, shift, old_fp, _context.pc, function }); */
        /* } */

        void Vm::do_function_call(bytecode_type instruction) {
            int func_reg = GET_REG1(instruction);
            int arg_count = GET_REG2(instruction);
            int ret_count = GET_REG3(instruction);

            // Save the old frame pointer
            auto old_fp = _context.fp;

            // Save the current stack pointer position as the new frame pointer
            _context.fp = _context.sp;

            auto function = get_function(_registers[func_reg].as_i32());
            KORE_DEBUG_VM_LOG("push call frame", function->name());

            // Reserve local stack space for the called function
            _context.sp += function->reg_count();

            int shift = 24;

            if (arg_count > 0) {
                CallFrame& frame = current_frame();

                // Get the next instruction containing the beginning of the
                // argument registers
                instruction = frame.code[_context.pc];

                // Push argument registers onto the call stack
                for (int i = 0; i < arg_count; ++i, shift -= 8) {
                    Reg dst_reg = _context.fp + i;
                    Reg src_reg = old_fp + (instruction >> shift) & 0xff;
                    move(dst_reg, src_reg);

                    if (shift == 0) {
                        shift = 24;
                        instruction = frame.code[++_context.pc];
                    }
                }
            }

            // Push a new call frame for the called function
            _call_frames.push_back(CallFrame{ ret_count, shift, old_fp, _context.pc, function });

            // Set the program counter to zero to start executing the start of
            // the called function's instructions
            _context.pc = 0;
        }

        Value Vm::pop() {
            return _registers[--_context.sp];
        }

        void Vm::do_function_return(bytecode_type instruction) {
            KORE_DEBUG_VM_LOG("pop call frame", std::string());

            CallFrame& frame = current_frame();
            int ret_count = GET_REG1(instruction);

            if (ret_count > 0) {
                auto shift = frame.shift;

                // Get the instruction at the return address
                auto& caller = get_caller();
                bytecode_type ret = caller.code[frame.old_pc++];

                // Copy return registers into destination registers in the previous call frame
                for (int i = 0, ret_shift = 8; i < ret_count; ++i, shift -= 8, ret_shift -= 8) {
                    // Get a source register from the return instruction
                    Reg src_reg = _context.fp + ((instruction >> ret_shift) & 0xff);

                    // Get a destination register from the return address instruction
                    // which is a register encoded in the original call instruction
                    Reg dst_reg = frame.old_fp + ((ret >> shift) & 0xff);

                    move(dst_reg, src_reg);

                    if (shift == 0) {
                        shift = 24;
                        instruction = caller.code[frame.old_pc++];
                    }

                    if (ret_shift == 0) {
                        ret_shift = 24;
                        ret = frame.code[_context.pc++];
                    }
                }
            }

            // Reset the stack pointer to the base of the current call frame
            // and beyond the old frame pointer and program counter. Restore
            // the old frame pointer and program counter
            _context.restore(frame);

            _call_frames.pop_back();
        }

        inline int Vm::top() {
            return _context.sp - 1;
        }

        void Vm::move(Reg dst_reg, Reg src_reg) {
            _registers[dst_reg] = _registers[src_reg];
        }

        void Vm::add_module(Module& module) {
            load_functions_from_module(module);

            auto module_name = module.name();

            // TODO: Does it make sense for the vm to own the module? Probably
            // not for the repl
            _modules.try_emplace(module_name, std::move(module));
            _context._current_module = &_modules[module_name];

        }
    }
}

#undef LOAD_OPCODE
#undef BINARY_ARITH_OPCODE
#undef BINARY_OP_CASES
#undef RELOP_OPCODE
#undef RELOP_CASES
#undef _KORE_DEBUG_VM
#undef KORE_DEBUG_VM_LOG
