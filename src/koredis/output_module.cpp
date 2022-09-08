#include <algorithm>

#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"
#include "logging/logging.hpp"
#include "utils/unused_parameter.hpp"
#include "version.hpp"

#include "decode_instruction.hpp"
#include "output_module.hpp"

namespace koredis {
    void output_constant_table(std::ostream& os, kore::Color color, kore::Module& module) {
        kore::section("constant table:i32", kore::Color::Magenta, kore::ColorAttribute::None, "");

        int table_index = 0;

        for (auto it = module.i32_constants_begin(); it != module.i32_constants_end(); ++it) {
            os << color << "| " << kore::ColorAttribute::Reset
               << table_index++ << " => " << *it << std::endl;
        }
    }

    bool compare_instruction_display_lengths(const Instruction ins1, const Instruction ins2) {
        return kore::bytecode_to_string(ins1.opcode()).size() < kore::bytecode_to_string(ins2.opcode()).size();
    }

    void output_function(std::ostream& os, kore::Color color, kore::CompiledObject& obj) {
        kore::section("function", kore::Color::Magenta, kore::ColorAttribute::Bold, obj.name().c_str());

        os << color << "| " << kore::ColorAttribute::Reset << "locals   : " << obj.locals_count() << std::endl;
        os << color << "| " << kore::ColorAttribute::Reset << "registers: " << obj.reg_count() << std::endl;
        os << color << "| " << kore::ColorAttribute::Reset << "code size: " << obj.code_size() << std::endl;
        os << color << "|" << kore::ColorAttribute::Reset << std::endl;

        auto decoded_instructions = decode_instructions(obj);

        /* auto max_ins_len = std::max_element( */
        /*     decoded_instructions.cbegin(), */
        /*     decoded_instructions.cend(), */
        /*     compare_instruction_display_lengths */
        /* ); */

        for (auto instruction : decoded_instructions) {
            os << color << "| " << kore::ColorAttribute::Reset << instruction << std::endl;
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

        output_constant_table(os, color, module);

        kore::info("functions (%d)", module.objects_count());

        for (auto it = module.objects_begin(); it != module.objects_end(); ++it) {
            output_function(os, color, *it->get());
        }
    }
}
