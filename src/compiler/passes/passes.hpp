#ifndef KORE_PASSES_HPP
#define KORE_PASSES_HPP

#include "compiler/passes/pass.hpp"

namespace kore {
    // Standard set of compiler passes (some are defined elsewhere)
    Pass get_parser_pass();
    Pass get_type_inference_pass();
    Pass get_type_checking_pass();
    Pass get_bytecode_codegen_pass();
    Pass get_bytecode_write_pass();

    std::vector<Pass> get_default_passes();
}

#endif // KORE_PASSES_HPP
