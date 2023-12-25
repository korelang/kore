#include "targets/bytecode/vm/builtins/builtins.hpp"
#include "targets/bytecode/vm/builtins/io.hpp"
#include "targets/bytecode/vm/builtins/math.hpp"
#include "types/type_category.hpp"

#include <array>

namespace kore {
    namespace vm {
        const std::array<BuiltinFunction, 2> _builtins = {
            {
                {
                    0,
                    print,
                    "print",
                    1,
                    0,
                    { Type::get_type_from_category(TypeCategory::Generic) },
                    { Type::get_type_from_category(TypeCategory::Void) }
                },
                {
                    1,
                    abs,
                    "abs",
                    1,
                    1,
                    { Type::get_type_from_category(TypeCategory::Integer32) },
                    { Type::get_type_from_category(TypeCategory::Integer32) },
                }
            },
        };

        // TODO: Can we construct this statically?
        std::unordered_map<std::string, BuiltinFunctionIndex> _builtins_by_name {
            { "print", 0 },
            { "abs", 1 },
        };
        
        /* static_assert(_builtins.size() == _builtins_by_name.size()); */

        const BuiltinFunction* get_builtin_function_by_index(BuiltinFunctionIndex idx) {
            if (idx >= _builtins.size()) {
                return nullptr;
            }

            return &_builtins[idx];
        }

        const BuiltinFunction* get_builtin_function_by_name(const std::string& name) {
            auto it = _builtins_by_name.find(name);

            if (it == _builtins_by_name.end()) {
                return nullptr;
            }

            return get_builtin_function_by_index(it->second);
        }
    }
}
