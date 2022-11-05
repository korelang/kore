#ifndef KORE_PASS_HPP
#define KORE_PASS_HPP

#include "passes/pass_context.hpp"
#include "passes/pass_result.hpp"

// More passes:
//
// Optimiser optimiser;
//
// // Eliminate dead code
// optimiser.optimise_dead_code();
// success_group("optimisation", "dead code");
//
// // Constant folding
// optimiser.optimise_constant_folding();
// success_group(2, args.verbosity, "optimisation", "constant folding");
//
// success(1, args.verbosity, "optimisation successful");
//
// // Add explicit type conversions
// ???

namespace kore {
    using CompilerPassRunFunc = PassResult (*)(PassContext& context);

    /// A single compiler pass
    struct Pass {
        const std::string name;
        CompilerPassRunFunc run;
    };
}

#endif // KORE_PASS_HPP
