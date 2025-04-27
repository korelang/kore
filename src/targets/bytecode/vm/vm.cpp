#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/compiled_object.hpp"
#include "targets/bytecode/module_loader.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"
#include "targets/bytecode/vm/config.hpp"
#include "targets/bytecode/vm/vm.hpp"
#include "types/function_type.hpp"

#define GET_REG_SHIFT(instruction, shift) (instruction >> shift) & 0xff;

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
        BINARY_OP(type, bool, >)\
        break;\
    \
    case Bytecode::Le##opcode_suffix:\
        BINARY_OP(type, bool, <=)\
        break;\
    \
    case Bytecode::Ge##opcode_suffix:\
        BINARY_OP(type, bool, >=)\
        break;\
    \
    case Bytecode::Eq##opcode_suffix:\
        BINARY_OP(type, bool, ==)\
        break;\
    \
    case Bytecode::Neq##opcode_suffix:\
        BINARY_OP(type, bool, !=)\
        break;\
}

#define _KORE_DEBUG_VM defined(KORE_DEBUG_VM) || defined(KORE_DEBUG)

#ifdef _KORE_DEBUG_VM
    #include "logging/logging.hpp"

    // TODO: Change to use debug_group
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
            this->fp = call_frame.old_fp;
            this->pc = call_frame.old_pc;
        }

        CallFrame::CallFrame(CompiledObject* obj)
            : CallFrame(0, 0, 0, 0, obj) {}

        CallFrame::CallFrame(
            int ret_count,
            int reg_count,
            const bytecode_type* code,
            std::size_t size
        )
            : ret_count(ret_count),
              reg_count(reg_count),
              code(code),
              size(size) {}

        CallFrame::CallFrame(
            int ret_count,
            int shift,
            std::size_t old_fp,
            std::size_t old_pc,
            const CompiledObject* obj
        )
            : ret_count(ret_count),
              reg_count(obj->reg_count()),
              shift(shift),
              old_fp(old_fp),
              old_pc(old_pc),
              code(obj->instructions()),
              size(obj->code_size()) {}

        const std::string Vm::log_group = "vm";

        Vm::Vm() {}

        Vm::~Vm() {}

        void Vm::run(const bytecode_type* code, std::size_t size) {
            if (!code || size < 1) {
                return;
            }

            _context.reset();
            main_call_frame_reg_count = current_frame()->reg_count;
            _context.sp += main_call_frame_reg_count;

            _running = true;
            _errored = false;

            // Main interpreter dispatch loop
            while (_call_frames.size() > 0 && _running) {
                auto instruction = current_frame()->code[_context.pc++];
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

                    case Bytecode::Cload: {
                        Reg reg = GET_REG1(instruction);
                        int index = GET_VALUE(instruction);
                        _registers[fp + reg] = _context._current_module->constant_table().get(index);
                        break;
                    }

                    case Bytecode::Gload: {
                        Reg reg = GET_REG1(instruction);
                        _registers[fp + reg] = _globals[GET_REG2(instruction)];
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

                    case Bytecode::ArrayAlloc: {
                        Reg reg = GET_REG1(instruction);
                        int size = GET_VALUE(instruction);

                        _registers[fp + reg] = Value::allocate_array(size);
                        break;
                    }

                    case Bytecode::ArrayGet: {
                        auto array = _registers[fp + GET_REG1(instruction)].as_array();
                        i32 idx = _registers[fp + GET_REG2(instruction)].as_i32();
                        Reg dst_reg = GET_REG3(instruction);

                        _registers[fp + dst_reg] = (*array)[idx];
                        break;
                    }

                    case Bytecode::ArraySet: {
                        auto array = _registers[fp + GET_REG1(instruction)].as_array();
                        int idx = _registers[fp + GET_REG2(instruction)].as_i32();
                        Value value = _registers[fp + GET_REG3(instruction)];

                        (*array)[idx] = value;
                        break;
                    }

                    case Bytecode::Free: {
                        /* Value value = _registers[fp + GET_REG1(instruction)]; */
                        /* value.free(); */
                        break;
                    }

                    case Bytecode::LoadBuiltin: {
                        Reg reg = GET_REG1(instruction);
                        int func_index = GET_VALUE(instruction);
                        auto builtin = get_builtin_function_by_index(func_index);
                        _registers[fp + reg] = Value::from_builtin_function(builtin);
                        break;
                    }

                    case Bytecode::LoadFunction: {
                        Reg reg = GET_REG1(instruction);
                        int func_index = GET_VALUE(instruction);
                        _registers[fp + reg] = Value::from_function(get_function(func_index));;
                        break;
                    }

                    case Bytecode::Call: {
                        Reg func_reg = GET_REG1(instruction);
                        auto callable = _registers[fp + func_reg].as_function_value();

                        if (callable.type == FunctionValueType::Ordinary) {
                            do_function_call(instruction, callable);
                        } else if (callable.type == FunctionValueType::Builtin) {
                            do_builtin_function_call(instruction, callable);
                        } else if (callable.type == FunctionValueType::Closure) {
                            vm_error("Closures are not yet supported");
                        }
                        break;
                    }

                    case Bytecode::Ret: {
                        do_function_return(instruction);
                        break;
                    }

                    default: {
                        vm_error_unknown_opcode(opcode);
                        break;
                    }
                }
            }

            /* if (_errored) { */
            /*     dump_call_stack(std::cerr); */
            /* } */

            _running = false;
            // TODO: Dump registers then register free memory here
        }

        void Vm::run(const std::vector<bytecode_type>& code) {
            run(code.data(), code.size());
        }

        void Vm::dump_registers(std::ostream& os) {
            // TODO: Track the highest register used by each call frame so we
            // can dump all call frames instead
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
            _call_frames.push_back(CallFrame{ main_object });

            run_compiled_object(main_object);
        }

        void Vm::run_compiled_object(CompiledObject* obj) {
            run(obj->instructions(), obj->code_size());
        }

        void Vm::load_functions_from_module(const Module& module) {
            // TODO: Move this lookup into the Module class?
            KORE_DEBUG_VM_LOG("module load", module.path());
            _loaded_functions.resize(_loaded_functions.size() + module.objects_count());

            for (auto it = module.objects_begin(); it != module.objects_end(); ++it) {
                _loaded_functions[(*it)->func_index()] = it->get();
            }
        }

        void Vm::allocate_globals(const Module& module) {
            _globals.resize(_globals.size() + module.global_indices_count());
        }

        bool Vm::allocate_local_stack(const CompiledObject* const obj) {
            auto reg_count = obj->reg_count();

            if (_context.sp + reg_count >= KORE_VM_MAX_REGISTERS) {
                vm_error("Stack-overflow, ran out of local stack");
                return false;
            }

            _context.sp += reg_count;
            return true;
        }

        void Vm::vm_error(const std::string& message) {
            error_group(log_group, "%s", message.c_str());
            /* critical_group(log_group, message); */
            _running = false;
            _errored = true;
        }

        void Vm::vm_error_unknown_opcode(Bytecode opcode) {
            // TODO: Emit current call frame information and function name
            auto message = "Unsupported bytecode at "
                + std::to_string(_context.pc)
                + ": "
                + std::to_string(opcode);

            vm_error(message);
        }

        void Vm::vm_fatal_error(const std::string& message) {
            error_group(log_group, "%s", message.c_str());
            /* critical_group(log_group, "%s", message.c_str()); */

            _running = false;
            _errored = true;
        }

        CompiledObject* Vm::get_function(int func_index) {
            return _loaded_functions[func_index];
        }

        CallFrame* Vm::current_frame() {
            #if KORE_DEBUG_VM
            if (_call_frames.size() == 0) {
                vm_fatal_error("No call frames when trying to get current call frame");
                return nullptr;
            }
            #endif

            return &_call_frames.back();
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

        const CallFrame* Vm::get_caller() {
            #ifdef _KORE_DEBUG_VM
                if (_call_frames.size() < 2) {
                    vm_error("No previous caller");
                    return nullptr;
                }
            #endif

            return &_call_frames[_call_frames.size() - 2];
        }

        int Vm::push_function_arguments(bytecode_type instruction, std::size_t old_fp) {
            int arg_count = GET_REG2(instruction);
            int shift = 24;

            if (arg_count > 0) {
                CallFrame* frame = current_frame();

                if (!frame) {
                    return 0;
                }

                // Get the instruction containing the beginning of the argument
                // registers
                instruction = frame->code[_context.pc];

                // Move argument registers into the callee's register window
                for (int idx = 0; idx < arg_count; ++idx, shift -= 8) {
                    Reg dst_reg = _context.fp + idx;
                    Reg src_reg = old_fp + GET_REG_SHIFT(instruction, shift);
                    move(dst_reg, src_reg);

                    if (shift == 0) {
                        shift = 24;
                        instruction = frame->code[_context.pc++];
                    }
                }
            }

            return shift;
        }

        void Vm::push_call_frame(CallFrame call_frame) {
            _call_frames.push_back(call_frame);

            // Set the program counter to zero to start executing the start of
            // the called function's instructions
            _context.pc = 0;
        }

        void Vm::pop_call_frame(const CallFrame& call_frame) {
            // Deallocate the current call frame by resetting the stack pointer
            // to the base of it just beyond the register window of the caller.
            _context.sp -= call_frame.reg_count;

            // Restore the old frame pointer and program counter
            _context.restore(call_frame);

            _call_frames.pop_back();
        }

        void Vm::do_function_call(bytecode_type instruction, const FunctionValue& callable) {
            // Save the old frame pointer
            auto old_fp = _context.fp;

            // Save the current stack pointer position as the new frame pointer
            _context.fp = _context.sp;

            KORE_DEBUG_VM_LOG("push call frame", callable.func->name());

            // Reserve local stack space for the called function
            if (!allocate_local_stack(callable.func)) {
                return;
            }

            int shift = push_function_arguments(instruction, old_fp);
            int ret_count = GET_REG3(instruction);

            push_call_frame(CallFrame{
                ret_count,
                shift,
                old_fp,
                _context.pc,
                callable.func,
            });
        }

        void Vm::do_builtin_function_call(
            bytecode_type instruction,
            const FunctionValue& callable
        ) {
            KORE_DEBUG_VM_LOG("call builtin", callable.builtin->name);

            _context.shift = push_function_arguments(instruction, _context.fp);
            callable.builtin->func(*this, &_registers[_context.fp]);

            // Move the instruction pointer if there were arguments and/or
            // return values since the instruction pointer was left on next
            // instruction by the caller which is either the next instruction
            // to execute or the one containing argument/return registers if
            // there were any
            if (callable.builtin->type->arity() + callable.builtin->type->return_arity() > 0) {
                ++_context.pc;
            }
        }

        Value Vm::pop() {
            return _registers[--_context.sp];
        }

        // TODO: Clean up register window by setting registers to a
        // sentinel/gravestore value
        void Vm::do_function_return(bytecode_type instruction) {
            KORE_DEBUG_VM_LOG("pop call frame", std::string());

            CallFrame* frame = current_frame();

            if (!frame) {
                return;
            }

            int ret_count = GET_REG1(instruction);

            // NOTE: Might want to specialse for returning one argument
            if (ret_count > 0) {
                auto caller_shift = frame->shift;
                auto callee_shift = 8; // Ret opcode + return count

                // Get the instruction at the return address
                auto caller = get_caller();

                if (!caller) {
                    return;
                }

                auto caller_ins = caller->code[frame->old_pc++];

                // Copy return registers into destination registers in the
                // previous call frame
                for (int idx = 0; idx < ret_count; caller_shift -= 8, callee_shift -= 8) {
                    // Get a source register from the caller instruction
                    Reg src_reg = _context.fp + GET_REG_SHIFT(instruction, callee_shift);

                    // Get a destination register from the return address instruction
                    // which is a register encoded in the original call instruction
                    Reg dst_reg = frame->old_fp + GET_REG_SHIFT(caller_ins, caller_shift);

                    move(dst_reg, src_reg);

                    // Exit loop if we are done and don't increment any program counters
                    // which might otherwise skip an instruction if the last register is
                    // aligned to a 4-byte boundary
                    if (++idx >= ret_count) {
                        break;
                    }

                    if (caller_shift == 0) {
                        caller_shift = 24;
                        instruction = caller->code[frame->old_pc++];
                    }

                    if (callee_shift == 0) {
                        callee_shift = 24;
                        caller_ins = frame->code[_context.pc++];
                    }
                }
            }

            pop_call_frame(*frame);
        }

        void Vm::set_return_value(const Value& value) {
            // Getting the current call frame here is the caller since calling a
            // builtin function does not push a call frame onto the stack
            CallFrame* frame = current_frame();

            if (!frame) {
                return;
            }

            auto instruction = frame->code[_context.pc];

            // Copy the value into a destination register in the caller's
            // register window. Get the destination register encoded in the
            // caller's call instruction
            Reg dst_reg = GET_REG_SHIFT(instruction, _context.shift);

            _registers[_context.fp + dst_reg] = value;

            // Move shift to the next return register
            _context.shift -= 8;

            if (_context.shift <= 0) {
                _context.shift = 24;
                ++_context.pc;
            }
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

        /* void Vm::dump_call_stack(std::ostream& os) { */
        /*     for (int idx = _call_frames.size(); idx >= 0; --idx) { */
        /*         auto& call_frame = _call_frames[idx]; */
        /*     } */
        /* } */
    }
}

#undef BINARY_OP_CASES
#undef RELOP_CASES
#undef KORE_DEBUG_VM_LOG
