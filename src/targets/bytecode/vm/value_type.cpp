#include "targets/bytecode/vm/value_type.hpp"

namespace kore {
    namespace vm {
        Value::Value() : tag(ValueTag::Bool) {}

        Value::~Value() {
            switch (tag) {
                case ValueTag::Bool:
                case ValueTag::I32:
                case ValueTag::I64:
                case ValueTag::F32:
                case ValueTag::F64:
                    // We don't allocate any memory for these values
                    break;

                case ValueTag::Array: {
                    #if KORE_VM_DEBUG
                        if (value._array == nullptr) {
                            throw std::runtime_error("Array is not allocated");
                        }
                    #endif

                    delete value._array;
                    break;
                }
            }
        }

        Value Value::from_bool(bool value) {
            auto _value = Value();

            _value.tag = ValueTag::Bool;
            _value.value._bool = value;

            return _value;
        }

        Value Value::from_i32(i32 value) {
            auto _value = Value();

            _value.tag = ValueTag::I32;
            _value.value._i32 = value;

            return _value;
        }

        Value Value::from_i64(i64 value) {
            auto _value = Value();

            _value.tag = ValueTag::I64;
            _value.value._i64 = value;

            return _value;
        }

        Value Value::from_f32(f32 value) {
            auto _value = Value();

            _value.tag = ValueTag::F32;
            _value.value._f32 = value;

            return _value;
        }

        Value Value::from_f64(f64 value) {
            auto _value = Value();

            _value.tag = ValueTag::F64;
            _value.value._f64 = value;

            return _value;
        }

        /* Value from_string(const std::string& str) { */
        /*     auto _value = Value(); */

        /*     _value.tag = ValueTag::Str; */
        /*     _value.value._str = str; */

        /*     return _value; */
        /* } */

        Value Value::allocate_array(std::size_t size) {
            auto _value = Value();

            _value.tag = ValueTag::Array;
            _value.value._array = ArrayValue::allocate(size);

            return _value;
        }

        std::ostream& operator<<(std::ostream& out, const Value& value) {
            switch (value.tag) {
                case ValueTag::Bool:
                    out << (value.value._bool ? "true" : "false");
                    break;

                case ValueTag::I32:
                    out << "i32(" << value.value._i32 << ")";
                    break;

                case ValueTag::I64:
                    out << "i64(" << value.value._i64 << ")";
                    break;

                case ValueTag::F32:
                    out << "f32(" << value.value._f32 << ")";
                    break;

                case ValueTag::F64:
                    out << "f64(" << value.value._f64 << ")";
                    break;

                /* case ValueTag::Str: */
                /*     out << "Value(" << value.value._str << ", str)"; */
                /*     break; */

                case ValueTag::Array: {
                    #if KORE_VM_DEBUG
                        if (value.value._array == nullptr) {
                            throw std::runtime_error("Array is not allocated");
                        }
                    #endif

                    out << (*value.value._array);
                    break;
                }
            }

            return out;
        }
    }
}
