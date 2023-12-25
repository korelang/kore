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
                case ValueTag::FunctionValue: {
                    // TODO: Deallocate environment when we have closures
                    // We don't allocate any memory for these values
                    break;
                }

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

        Value Value::from_builtin_function(const BuiltinFunction* builtin) {
            auto _value = Value();

            _value.tag = ValueTag::FunctionValue;
            _value.value._function_value.type = FunctionValueType::Builtin;
            _value.value._function_value.builtin = builtin;

            return _value;
        }

        Value Value::from_function(const CompiledObject* const function) {
            auto _value = Value();

            _value.tag = ValueTag::FunctionValue;
            _value.value._function_value.type = FunctionValueType::Ordinary;
            _value.value._function_value.func = function;

            return _value;
        }

        bool operator==(const Value& value1, const Value& value2) {
            if (value1.tag != value2.tag) {
                return false;
            }

            switch (value1.tag) {
                case ValueTag::Bool: {
                    return value1.value._bool == value2.value._bool;
                }

                case ValueTag::I32:{
                    return value1.value._i32 == value2.value._i32;
                }

                case ValueTag::I64:{
                    return value1.value._i64 == value2.value._i64;
                }

                case ValueTag::F32:{
                    return value1.value._f32 == value2.value._f32;
                }

                case ValueTag::F64: {
                    return value1.value._f64 == value2.value._f64;
                }

                case ValueTag::Array: {
                    #if KORE_VM_DEBUG
                        if (value._array == nullptr) {
                            throw std::runtime_error("Array is not allocated");
                        }

                        if (value2.value._array == nullptr) {
                            throw std::runtime_error("Compared array is not allocated");
                        }
                    #endif

                    return *value1.value._array == *value2.value._array;
                }

                case ValueTag::FunctionValue: {
                    // FIX:
                    return value1.value._function_value.type == value2.value._function_value.type;
                }
            }
        }

        void Value::display_value(std::ostream& os) const {
            switch (tag) {
                case ValueTag::Bool: {
                    os << (value._bool ? "true" : "false");
                    break;
                }

                case ValueTag::I32: {
                    os << value._i32;
                    break;
                }

                case ValueTag::I64: {
                    os << value._i64;
                    break;
                }

                case ValueTag::F32: {
                    os << value._f32;
                    break;
                }

                case ValueTag::F64: {
                    os << value._f64;
                    break;
                }

                /* case ValueTag::Str: */
                /*     out << "Value(" << value.value._str << ", str)"; */
                /*     break; */

                case ValueTag::Array: {
                    #if KORE_VM_DEBUG
                        if (value._array == nullptr) {
                            throw std::runtime_error("Array is not allocated");
                        }
                    #endif

                    os << (*value._array);
                    break;
                }

                case ValueTag::FunctionValue: {
                    #if KORE_VM_DEBUG
                        // FIX:
                    #endif

                    os << value._function_value;
                    break;
                }
            }
        }

        std::ostream& operator<<(std::ostream& out, const Value& value) {
            switch (value.tag) {
                case ValueTag::Bool: {
                    out << (value.value._bool ? "true" : "false");
                    break;
                }

                case ValueTag::I32: {
                    out << "<i32 " << value.value._i32 << ">";
                    break;
                }

                case ValueTag::I64: {
                    out << "<i64 " << value.value._i64 << ">";
                    break;
                }

                case ValueTag::F32: {
                    out << "<f32 " << value.value._f32 << ">";
                    break;
                }

                case ValueTag::F64: {
                    out << "<f64 " << value.value._f64 << ">";
                    break;
                }

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

                case ValueTag::FunctionValue: {
                    #if KORE_VM_DEBUG
                        if (value.value._function_value.index < 0) {
                            throw std::runtime_error(
                                "Function value contains negative function index"
                            );
                        }

                        if (value.value._function_value.obj == nullptr) {
                            throw std::runtime_error(
                                "Function value points to null compiled function"
                            );
                        }
                    #endif

                    out << value.value._function_value;
                    break;
                }
            }

            return out;
        }
    }
}
