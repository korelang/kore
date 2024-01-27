#ifndef KORE_PASS_RESULT_HPP
#define KORE_PASS_RESULT_HPP

#include <vector>

#include "diagnostics/diagnostic.hpp"
#include "errors/error.hpp"

namespace kore {
    struct PassResult {
        const bool proceed;
        const std::vector<Diagnostic> diagnostics;
    };
}

#endif // KORE_PASS_RESULT_HPP
