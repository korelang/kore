#include "logging/color_attributes.hpp"
#include "logging/colors.hpp"
#include "output_module.hpp"
#include "utils/unused_parameter.hpp"
#include "version.hpp"

namespace koredis {
    void output_module(std::ostream& os, kore::Module& module, bool colors, bool porcelain, int verbosity) {
        UNUSED_PARAM(porcelain);
        UNUSED_PARAM(verbosity);

        if (colors) {
            os << kore::Color::White;
        }

        kore::Version compiler_version = module.get_compiler_version();
        kore::Version bytecode_version = module.get_bytecode_version();

        os << "compiler version: " << compiler_version << std::endl;
        os << "bytecode version: " << bytecode_version << std::endl;
    }
}
