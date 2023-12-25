#ifndef KORE_BUILTINS_MATH_HPP
#define KORE_BUILTINS_MATH_HPP

#include "targets/bytecode/vm/vm.hpp"

namespace kore {
    namespace vm {
        /// Return the absolute value of a value
        void abs(Vm& vm, const Value* args);
    }
}

#endif // KORE_BUILTINS_MATH_HPP
