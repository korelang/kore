#ifndef KORE_PASS_RESULT_HPP
#define KORE_PASS_RESULT_HPP

#include <vector>

#include "errors/error.hpp"

namespace kore {
    struct PassResult {
        const bool proceed;
        const std::vector<errors::Error> errors;
    };
}

#endif // KORE_PASS_RESULT_HPP
