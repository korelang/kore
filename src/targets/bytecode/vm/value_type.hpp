#ifndef KORE_VALUE_TYPE_HPP
#define KORE_VALUE_TYPE_HPP

#include "internal_value_types.hpp"
#include "targets/bytecode/vm/values/array_value.hpp"
#include "targets/bytecode/vm/values/function_value.hpp"

#include <ostream>

namespace kore {
    namespace vm {
        enum class ValueTag : std::uint8_t {
            Bool = 0,
            I32,
            I64,
            F32,
            F64,
            Array,
            FunctionValue,
            /* Str, */
        };

        /// The types for the vm's runtime values implemented
        /// as a tagged union
        struct Value {
            ValueTag tag;

            union _Value {
                bool _bool;
                i32 _i32;
                i64 _i64;
                f32 _f32;
                f64 _f64;
                ArrayValue* _array;
                FunctionValue _function_value;
                /* std::string _str; */
            } value;

            Value();

            ~Value();

            inline bool as_bool() const {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::Bool) {
                    throw std::runtime_error("Not a boolean value");
                }
                #endif

                return value._bool;
            }

            inline i32 as_i32() const {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::I32) {
                    throw std::runtime_error("Not an i32 value");
                }
                #endif

                return value._i32;
            }

            inline i64 as_i64() const {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::I64) {
                    throw std::runtime_error("Not an i64 value");
                }
                #endif

                return value._i64;
            }

            inline f32 as_f32() const {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::F32) {
                    throw std::runtime_error("Not an f32 value");
                }
                #endif

                return value._f32;
            }

            inline f64 as_f64() const {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::F64) {
                    throw std::runtime_error("Not an f64 value");
                }
                #endif

                return value._f64;
            }

            /* inline std::string as_str() const { */
            /*     #if KORE_VM_DEBUG */
            /*     if (tag != ValueTag::Str) { */
            /*         throw std::runtime_error("Not a str value"); */
            /*     } */
            /*     #endif */

            /*     return value._str; */
            /* } */

            inline ArrayValue* as_array() {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::Array) {
                    throw std::runtime_error("Not an array value");
                }
                #endif

                return value._array;
            }

            inline const ArrayValue* as_array() const {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::Array) {
                    throw std::runtime_error("Not an array value");
                }
                #endif

                return value._array;
            }

            inline FunctionValue as_function_value() {
                #if KORE_VM_DEBUG
                if (tag != ValueTag::FunctionValue) {
                    throw std::runtime_error("Not a function value");
                }
                #endif

                return value._function_value;
            }

            /// Print the value as it would be printed by 'print'
            void display_value(std::ostream& os) const;

            static Value from_bool(bool value);
            static Value from_i32(i32 value);
            static Value from_i64(i64 value);
            static Value from_f32(f32 value);
            static Value from_f64(f64 value);
            /* static Value from_string(const std::string& str); */
            static Value allocate_array(std::size_t size);
            static Value from_builtin_function(const BuiltinFunction* builtin);
            static Value from_function(const CompiledObject* const function);
        };

        bool operator==(const Value& value1, const Value& value2);

        std::ostream& operator<<(std::ostream& out, const Value& value);
    }
}

// Custom std::hash specialisation for vm values
template<>
struct std::hash<kore::vm::Value> {
    std::size_t operator()(const kore::vm::Value& value) const noexcept {
        switch (value.tag) {
            case kore::vm::ValueTag::Bool: {
                return std::hash<bool>{}(value.as_bool());
            }

            case kore::vm::ValueTag::I32: {
                return std::hash<kore::i32>{}(value.as_i32());
            }

            case kore::vm::ValueTag::I64: {
                return std::hash<kore::i64>{}(value.as_i64());
            }

            case kore::vm::ValueTag::F32: {
                return std::hash<kore::f32>{}(value.as_f32());
            }

            case kore::vm::ValueTag::F64: {
                return std::hash<kore::f64>{}(value.as_f64());
            }

            case kore::vm::ValueTag::Array: {
                // FIXME: Use a better hash function than size
                return std::hash<std::size_t>{}(value.as_array()->size());
            }

            case kore::vm::ValueTag::FunctionValue: {
                return std::hash<kore::i64>{}(value.as_i64());
            }
        }
    }
};
#endif // KORE_VALUE_TYPE_HPP
