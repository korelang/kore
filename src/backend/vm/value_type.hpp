#ifndef KORE_VALUE_TYPE_HPP
#define KORE_VALUE_TYPE_HPP

#include "frontend/internal_value_types.hpp"

namespace kore {
    enum class ValueTag {
        I32,
        I64,
        F32,
        F64,
        Str,
    };

    /// The types for the vm's runtime values implemented
    /// as a tagged union
    struct Value {
        ValueTag tag;

        union _Value {
            i32 _i32;
            i64 _i64;
            f32 _f32;
            f64 _f64;
        } value;

        inline i32 as_i32() {
            #if KORE_VM_DEBUG
            if (tag != ValueTag::I32) {
                throw std::runtime_error("Not an i32 value");
            }
            #endif

            return value._i32;
        }

        inline i64 as_i64() {
            #if KORE_VM_DEBUG
            if (tag != ValueTag::I64) {
                throw std::runtime_error("Not an i64 value");
            }
            #endif

            return value._i64;
        }

        inline f32 as_f32() {
            #if KORE_VM_DEBUG
            if (tag != ValueTag::f32) {
                throw std::runtime_error("Not an f32 value");
            }
            #endif

            return value._f32;
        }

        inline f64 as_f64() {
            #if KORE_VM_DEBUG
            if (tag != ValueTag::f64) {
                throw std::runtime_error("Not an f64 value");
            }
            #endif

            return value._f64;
        }
    };

    Value from_i32(i32 value);
    Value from_i64(i64 value);
    Value from_f32(f32 value);
    Value from_f64(f64 value);
}

#endif // KORE_VALUE_TYPE_HPP
