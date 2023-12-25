#include "targets/bytecode/vm/builtins/io.hpp"
#include "utils/unused_parameter.hpp"

#include <iostream>

namespace kore {
    namespace vm {
        void print(Vm& vm, const Value* args) {
            UNUSED_PARAM(vm);

            args[0].display_value(std::cout);
            std::cout << std::endl;
        }
    }
}
