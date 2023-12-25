#include "function_value.hpp"
#include "targets/bytecode/compiled_object.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"

namespace kore {
    namespace vm {
        std::ostream& operator<<(std::ostream& os, const FunctionValue& value) {
            switch (value.type) {
                case FunctionValueType::Builtin: {
                    return os << "<builtin "
                        << std::quoted(value.builtin->name.c_str(), '\'')
                        << ">";
                }

                case FunctionValueType::Ordinary: {
                    return os << "<func "
                        << std::quoted(value.func->name().c_str(), '\'')
                        << ">";
                }

                default:
                    return os;
            }
        }
    }
}
