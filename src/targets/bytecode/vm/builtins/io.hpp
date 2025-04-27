#ifndef KORE_BUILTINS_IO_HPP
#define KORE_BUILTINS_IO_HPP

#include "targets/bytecode/vm/vm.hpp"

namespace kore {
    namespace vm {
        void print(Vm& vm, const Value* args);
    }
}

#endif // KORE_BUILTINS_IO_HPP
