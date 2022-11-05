#ifndef KORE_COMPILER_HPP
#define KORE_COMPILER_HPP

#include <vector>

#include "passes/pass.hpp"
#include "passes/pass_context.hpp"

namespace kore {
    /// A compiler is a set of compiler passes run sequentially
    class Compiler final {
        public:
            Compiler(
                ParsedCommandLineArgs args,
                const std::vector<Pass>& passes
            );

            int run_passes();

            void print_errors(const Pass& pass, PassResult result);

        private:
            PassContext _context;
            std::vector<Pass> _passes;
    };
}

#endif // KORE_COMPILER_HPP
