#ifndef KOREDIS_DUMP_MODULE_HPP
#define KOREDIS_DUMP_MODULE_HPP

#include "logging/color_spec.hpp"
#include "targets/bytecode/module.hpp"

#include <ostream>

namespace koredis {
    struct DumpConfig {
        bool colors;
        bool porcelain;
        int verbosity;
        kore::ColorSpec section_color_spec;
        kore::ColorSpec opcode_color_spec;
        kore::ColorSpec version_color_spec;
    };

    void dump_module(std::ostream& os, kore::Module& module, const DumpConfig& config);
}

#endif // KOREDIS_DUMP_MODULE_HPP
