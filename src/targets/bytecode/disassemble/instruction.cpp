#include "instruction.hpp"
#include "targets/bytecode/module_load_error.hpp"
#include "targets/bytecode/register.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"

namespace koredis {
    const std::string register_symbol = "@";
    const std::string constant_index_symbol = "#";

    struct reg {
        kore::Reg _reg;

        reg(kore::Reg reg) : _reg(reg) {}
    };

    struct regs {
        const std::vector<kore::Reg>& _regs;

        regs(const std::vector<kore::Reg>& regs) : _regs(regs) {}
    };

    struct constant {
        int _index;

        constant(int index) : _index(index) {}
    };

    template<typename CharT, typename Traits>
    decltype(auto) operator<<(std::basic_ostream<CharT, Traits>& os, reg reg_formatter) {
        return os << register_symbol << reg_formatter._reg;
    }

    template<typename CharT, typename Traits>
    decltype(auto) operator<<(std::basic_ostream<CharT, Traits>& os, regs regs_formatter) {
        auto& _regs = regs_formatter._regs;

        for (std::size_t i = 0; i < _regs.size(); ++i) {
            os << reg(_regs[i]);

            if (i < _regs.size() - 1) {
                os << " ";
            }
        }

        return os;
    }

    template<typename CharT, typename Traits>
    decltype(auto) operator<<(std::basic_ostream<CharT, Traits>& os, constant constant_formatter) {
        return os << constant_index_symbol << constant_formatter._index;
    }

    std::ostream& format_register_list(
        std::ostream& os,
        const std::vector<kore::Reg> registers
    ) {
        for (auto reg : registers) {
            os << " " << register_symbol << reg;
        }

        return os;
    }

    std::ostream& format_registers(std::ostream& os, Instruction instruction, kore::Module& module) {
        // Special-case output for more complex opcodes
        auto instruction_type = instruction.value.type;

        if (auto ins_type = std::get_if<kore::kir::OneRegister>(&instruction_type)) {
            if (instruction.value.opcode == kore::Bytecode::Noop) {
                return os;
            }

            os << " " << reg(ins_type->reg);
        } else if (auto ins_type = std::get_if<kore::kir::TwoRegisters>(&instruction_type)) {
            if (instruction.value.opcode == kore::Bytecode::Gstore) {
                os << " " << constant(ins_type->reg1) << " " << reg(ins_type->reg2);
            } else {
                os << " " << reg(ins_type->reg1) << " " << reg(ins_type->reg2);
            }
        } else if (auto ins_type = std::get_if<kore::kir::ThreeRegisters>(&instruction_type)) {
            os << " " << reg(ins_type->reg1)
               << " " << reg(ins_type->reg2)
               << " " << reg(ins_type->reg3);
        } else if (auto ins_type = std::get_if<kore::kir::RegisterAndValue>(&instruction_type)) {
            auto opcode = instruction.value.opcode;
            auto value = ins_type->value;
            os << " " << reg(ins_type->reg);

            if (opcode == kore::Bytecode::JumpIf || opcode == kore::Bytecode::JumpIfNot) {
                auto target_pos = instruction.byte_pos + value;

                os << " " << ins_type->value << " [target: " << target_pos << "]";
            } else if (opcode == kore::Bytecode::LoadBool) {
                os << " " << (value == 1 ? "true" : "false");
            } else if (opcode == kore::LoadFunction) {
                auto index = ins_type->value;
                auto obj = module.get_function_by_index(index);

                // TODO: Perhaps move this check to decode_instruction so
                // that this function is only concerned with formatting
                if (!obj) {
                    throw kore::ModuleLoadError("Could not find function from index");
                }

                auto name = obj->name();

                os << " " << index << " [" << name << "]";
            } else if (opcode == kore::Bytecode::LoadBuiltin) {
                auto index = ins_type->value;
                auto builtin = kore::vm::get_builtin_function_by_index(index);

                if (!builtin) {
                    // TODO: Provide index in message
                    throw kore::ModuleLoadError("Could not find builtin function from index");
                }

                os << " " << index << " [" << builtin->name << " (builtin)]";
            } else if (opcode == kore::Bytecode::ArrayAlloc) {
                os << " " << value;
            } else if (opcode == kore::Bytecode::Cload) {
                os << " " << constant(value) << " [" << module.constant_table().get(value) << "]";
            } else {
                os << " " << constant(value);
            }
        } else if (auto ins_type = std::get_if<kore::kir::Value>(&instruction_type)) {
            auto value = ins_type->value;
            auto target_pos = instruction.byte_pos + value;

            os << " " << value << " [target: " << target_pos << "]";
        } else if (auto ins_type = std::get_if<kore::kir::CallV>(&instruction_type)) {
            os << " " << reg(ins_type->func_index);

            if (ins_type->arg_registers.size() > 0) {
                os << " " << regs(ins_type->arg_registers);
            }

            if (ins_type->ret_registers.size() > 0) {
                os << " " << regs(ins_type->ret_registers);
            }

            os << " [args: "
               << ins_type->arg_registers.size()
               << ", returns: "
               << ins_type->ret_registers.size() << "]";
        } else if (auto ins_type = std::get_if<kore::kir::ReturnV>(&instruction_type)) {
            os << " " << regs(ins_type->registers);
        }

        return os;
    }

    /* std::ostream& operator<<(std::ostream& os, const Instruction instruction) { */
    /*     os << instruction.byte_pos << ": " << bytecode_to_string(instruction.value.opcode); */
        
    /*     return format_registers(os, instruction); */
    /* } */
}
