#ifndef KORE_FUNCTION_VALUE_HPP
#define KORE_FUNCTION_VALUE_HPP

#include <ostream>
#include <vector>
#include "targets/bytecode/compiled_object.hpp"

namespace kore {
    namespace vm {
        /// A function value is a user-declared, non-closure function that
        /// contains a function index and a pointer to the compiled function
        struct FunctionValue {
            int index;
            const CompiledObject* obj;
        };

        std::ostream& operator<<(std::ostream& out, const FunctionValue& value);
    }
}

#endif // KORE_FUNCTION_VALUE_HPP
