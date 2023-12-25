#ifndef KORE_FUNCTION_VALUE_HPP
#define KORE_FUNCTION_VALUE_HPP

#include <ostream>
#include <type_traits>
#include <vector>

namespace kore {
    class CompiledObject;

    namespace vm {
        struct BuiltinFunction;

        enum class FunctionValueType {
            Ordinary = 0,
            Builtin,
            Closure
        };

        constexpr auto MaxFunctionValue = static_cast<
            std::underlying_type_t<FunctionValueType>
        >(FunctionValueType::Closure);

        /// A function value is a user-declared, non-closure function that
        /// contains a function index and a pointer to the compiled function
        struct FunctionValue {
            FunctionValueType type;
            const BuiltinFunction* builtin;
            const kore::CompiledObject* func;
        };

        std::ostream& operator<<(std::ostream& os, const FunctionValue& value);
    }
}

#endif // KORE_FUNCTION_VALUE_HPP
