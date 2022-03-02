#include "backend/vm/value_type.hpp"

namespace kore {
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
}
