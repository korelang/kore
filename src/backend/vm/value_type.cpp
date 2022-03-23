#include "backend/vm/value_type.hpp"

namespace kore {
    Value from_bool(bool value) {
        return Value{ ValueTag::Bool, { ._bool = value } };
    }

    Value from_i32(i32 value) {
        return Value{ ValueTag::I32, { ._i32 = value } };
    }

    Value from_i64(i64 value) {
        return Value{ ValueTag::I64, { ._i64 = value } };
    }

    Value from_f32(f32 value) {
        return Value{ ValueTag::F32, { ._f32 = value } };
    }

    Value from_f64(f64 value) {
        return Value{ ValueTag::F64, { ._f64 = value } };
    }

    std::ostream& operator<<(std::ostream& out, const Value& value) {
        switch (value.tag) {
            case ValueTag::Bool:
                out << "Value(" << value.value._bool << ", bool)";
                break;

            case ValueTag::I32:
                out << "Value(" << value.value._i32 << ", i32)";
                break;

            case ValueTag::I64:
                out << "Value(" << value.value._i64 << ", i64)";
                break;

            case ValueTag::F32:
                out << "Value(" << value.value._f32 << ", f32)";
                break;

            case ValueTag::F64:
                out << "Value(" << value.value._f64 << ", f64)";
                break;

            case ValueTag::Str:
                break;
        }

        return out;
    }
}
