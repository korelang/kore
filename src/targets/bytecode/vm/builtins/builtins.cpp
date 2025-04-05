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
                    Type::make_function_type(
                        // TODO: Change to TypeCategory::Generic when it is implemented
                        { Type::get_type_from_category(TypeCategory::Integer32) },
                        { Type::get_type_from_category(TypeCategory::Void) }
                    )
                },
                {
                    1,
                    abs,
                    "abs",
                    Type::make_function_type(
                        { Type::get_type_from_category(TypeCategory::Integer32) },
                        { Type::get_type_from_category(TypeCategory::Integer32) }
                    )
                }
            },
        };

        // TODO: Can we construct this statically?
        std::unordered_map<std::string, BuiltinFunctionIndex> _builtins_by_name {
            { "print", 0 },
            { "abs", 1 },
        };
        
        /* static_assert(_builtins.size() == _builtins_by_name.size()); */

        int builtin_function_count() {
            return _builtins.size();
        }

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
