#ifndef KORE_BUILTINS_HPP
#define KORE_BUILTINS_HPP

#include "targets/bytecode/vm/vm.hpp"

#include <functional>

namespace kore {
    namespace vm {
        using BuiltinFunctionIndex = unsigned int;

        // Pass a reference to the VM so builtin functions can access VM state
        // and set return values in the register window
        using BuiltinFunctionPointer = std::function<void(Vm& vm, const Value* args)>;

        struct BuiltinFunction {
            BuiltinFunctionIndex index;
            BuiltinFunctionPointer func;
            std::string name;
            int arity;
            int ret_count;
            std::vector<const Type*> arg_types;
            std::vector<const Type*> ret_types;
        };

        const BuiltinFunction* get_builtin_function_by_index(BuiltinFunctionIndex idx);
        const BuiltinFunction* get_builtin_function_by_name(const std::string& name);
    }
}

#endif // KORE_BUILTINS_HPP
