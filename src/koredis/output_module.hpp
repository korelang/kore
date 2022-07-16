#ifndef KOREDIS_OUTPUT_MODULE_HPP
#define KOREDIS_OUTPUT_MODULE_HPP

#include <ostream>

#include "module.hpp"

namespace koredis {
    void output_module(std::ostream& os, kore::Module& module, bool colors, bool porcelain, int verbosity);
}

#endif // KOREDIS_OUTPUT_MODULE_HPP
