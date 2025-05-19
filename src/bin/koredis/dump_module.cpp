#include "dump_module.hpp"
#include "logging/color.hpp"
#include "logging/color_attributes.hpp"
#include "logging/logging.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/disassemble/decode_instruction.hpp"
#include "targets/bytecode/module_load_error.hpp"
#include "utils/unused_parameter.hpp"
#include "version.hpp"

#include <algorithm>
#include <iomanip>

namespace koredis {
    std::string constant_table_tag_to_string(kore::vm::ValueTag tag) {
        switch (tag) {
            case kore::vm::ValueTag::I32: return "i32";
            case kore::vm::ValueTag::I64: return "i64";
            case kore::vm::ValueTag::F32: return "f32";
            case kore::vm::ValueTag::F64: return "f64";
            default: return "<error_tag>";
        }
    }

    void dump_value(kore::vm::Value& value, std::ostream& os) {
        switch (value.tag) {
            case kore::vm::ValueTag::Bool:
            case kore::vm::ValueTag::I32:
            case kore::vm::ValueTag::I64:
            case kore::vm::ValueTag::F32:
            case kore::vm::ValueTag::F64: {
                os << value;
                break;
            }

            default:
                // NOTE: Make exceptions that can take a group (e.g. codegen?)
                throw std::runtime_error("Unsupported value tag");
        }
    }

    void dump_constant_table(
        std::ostream& os,
        const kore::Module& module,
        kore::vm::ValueTag tag,
        const DumpConfig& dump_config
    ) {
        auto table = module.constant_table();

        kore::section(
            "constant table",
            dump_config.section_color_spec,
            0,
            "%s (%d)",
            constant_table_tag_to_string(tag).c_str(),
            table.size()
        );

        if (table.size() == 0) {
            return;
        }

        for (auto it = table.tagged_cbegin(tag); it != table.tagged_cend(tag); ++it) {
            os << " #" << *it << " => ";

            auto value = table.get(*it);
            dump_value(value, os);

            os << std::endl;
        }
    }

    void dump_function(
        std::ostream& os,
        kore::CompiledObject& obj,
        kore::Module& module,
        const DumpConfig& dump_config
    ) {
        // TODO: Make colors configurable instead of hardcoded
        kore::section(
            "function",
            dump_config.section_color_spec,
            0,
            "%s (registers: %d, code size: %d)",
            obj.name().c_str(),
            obj.reg_count(),
            obj.code_size()
        );

        // TODO: Make a dumper class for a more structured way to output these
        // formats? Like JSON etc.
        if (obj.is_main_object()) {
            os << "  Source location: " << module.path();
        } else {
            os << "  Source location: " << module.path() << ", " << obj.location();
        }

        os << std::endl << std::endl;

        auto attr_reset = kore::ColorAttribute::Reset;
        auto decoded_instructions = decode_instructions(obj);

        auto longest_instruction = std::max_element(
            decoded_instructions.cbegin(),
            decoded_instructions.cend(),
            [](Instruction ins1, Instruction ins2) {
                return kore::bytecode_to_string(ins1.value.opcode).size() < kore::bytecode_to_string(ins2.value.opcode).size();
            }
        );

        auto longest_instruction_len = kore::bytecode_to_string(longest_instruction->value.opcode).size();

        for (auto instruction : decoded_instructions) {
            os << dump_config.opcode_color_spec << "  " << attr_reset
               << std::left << std::setw(4)
               << instruction.byte_pos
               << dump_config.opcode_color_spec
               << std::left << std::setw(longest_instruction_len)
               << kore::bytecode_to_string(instruction.value.opcode) << attr_reset;

            format_registers(os, instruction, module);

            os << std::endl;
        }

        os << std::endl;
    }

    void dump_module(std::ostream& os, kore::Module& module, const DumpConfig& dump_config) {
        // kore::Version compiler_version = module.get_compiler_version();
        // kore::Version bytecode_version = module.get_bytecode_version();
        //
        auto version_color_spec = dump_config.version_color_spec;

        // kore::info("compiler version: %s", compiler_version);
        // kore::info("bytecode version: %s", bytecode_version);

        kore::section(
            "globals",
            dump_config.section_color_spec,
            0,
            "%d",
            module.global_indices_count()
        );

        dump_constant_table(os, module, kore::vm::ValueTag::I32, dump_config);

        kore::section(
            "functions",
            dump_config.section_color_spec,
            0,
            "%d",
            module.objects_count()
        );

        for (auto it = module.objects_begin(); it != module.objects_end(); ++it) {
            dump_function(os, *it->get(), module, dump_config);
        }
    }
}
