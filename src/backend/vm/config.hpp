#ifndef KORE_VM_CONFIG_HPP
#define KORE_VM_CONFIG_HPP

namespace kore {
    namespace vm {
        #ifndef KORE_VM_MAX_REGISTERS
            constexpr int KORE_VM_MAX_REGISTERS = 256;
        #else
            constexpr int KORE_VM_MAX_REGISTERS = KORE_VM_MAX_REGISTERS;
        #endif

        /* #ifndef KORE_VM_CALLSTACK_SIZE */
        /*     constexpr int KORE_VM_CALLSTACK_SIZE = 256; */
        /* #endif */
    }
}

#endif // KORE_VM_CONFIG_HPP
