#include "targets/bytecode/vm/builtins/math.hpp"

#include <cmath>

namespace kore {
    namespace vm {
        void abs(Vm& vm, const Value* args) {
            auto value = args[0];
            Value abs_value;

            switch (value.tag) {
                case ValueTag::I32: {
                    abs_value = Value::from_i32(std::abs(value.as_i32()));
                    break;
                }

                case ValueTag::I64: {
                    abs_value = Value::from_i64(std::abs(value.as_i64()));
                    break;
                }

                case ValueTag::F32: {
                    abs_value = Value::from_f32(std::abs(value.as_f32()));
                    break;
                }

                case ValueTag::F64: {
                    abs_value = Value::from_f64(std::abs(value.as_f64()));
                    break;
                }

                default:
                    vm.vm_fatal_error("math.abs applied to non-numeric value");
                    break;
            }

            vm.set_return_value(abs_value);
        }
    }
}
