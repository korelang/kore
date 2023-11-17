#include <algorithm>
#include <iomanip>

#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"
#include "logging/logging.hpp"
#include "output_module.hpp"
#include "targets/bytecode/disassemble/decode_instruction.hpp"
#include "utils/unused_parameter.hpp"
#include "version.hpp"

namespace koredis {
    std::string constant_table_tag_to_string(kore::ConstantTableTag tag) {
        switch (tag) {
            case kore::ConstantTableTag::I32: return "i32";
            case kore::ConstantTableTag::I64: return "i64";
            case kore::ConstantTableTag::F32: return "f32";
            case kore::ConstantTableTag::F64: return "f64";
            default: return "<error_tag>";
        }
    }

    bool compare_instruction_display_lengths(const Instruction ins1, const Instruction ins2) {
        return kore::bytecode_to_string(ins1.opcode()).size() < kore::bytecode_to_string(ins2.opcode()).size();
    }

    void output_function(std::ostream& os, kore::Color color, kore::CompiledObject& obj) {
        kore::section("function", kore::Color::Magenta, kore::ColorAttribute::Bold, 0, obj.name().c_str());
        auto attr = kore::ColorAttribute::Reset;

        os << color << "  " << attr << "registers: " << obj.reg_count() << std::endl;
        os << color << "  " << attr << "code size: " << obj.code_size() << std::endl;
        os << color << "" << attr << std::endl;

        auto decoded_instructions = decode_instructions(obj);
        auto opcode_color = kore::Color::Blue;

        for (auto instruction : decoded_instructions) {
            os << color << "  " << attr
               << instruction.byte_position() << "  "
               << std::setw(10) << std::left
               << opcode_color << instruction.name() << attr
               << instruction.registers_as_string()
               << std::endl;
        }
    }

    void output_module(std::ostream& os, kore::Module& module, bool colors, bool porcelain, int verbosity) {
        UNUSED_PARAM(porcelain);
        UNUSED_PARAM(verbosity);

        kore::Color color = colors ? kore::Color::White : kore::Color::None;

        kore::Version compiler_version = module.get_compiler_version();
        kore::Version bytecode_version = module.get_bytecode_version();

        os << color << "compiler version: " << kore::ColorAttribute::Reset << compiler_version << std::endl;
        os << color << "bytecode version: " << kore::ColorAttribute::Reset << bytecode_version << std::endl;
        os << std::endl;

        kore::section(
            "globals",
            kore::Color::Magenta,
            kore::ColorAttribute::Bold,
            0,
            "%d",
            module.global_indices_count()
        );

        output_constant_table(os, color, module.i32_constant_table());
        /* output_constant_table(os, color, module.i64_constant_table()); */
        /* output_constant_table(os, color, module.f32_constant_table()); */
        /* output_constant_table(os, color, module.f64_constant_table()); */

        kore::section("functions", kore::Color::Magenta, kore::ColorAttribute::Bold, 0, "%d", module.objects_count());

        for (auto it = module.objects_begin(); it != module.objects_end(); ++it) {
            output_function(os, color, *it->get());
        }
    }
}
