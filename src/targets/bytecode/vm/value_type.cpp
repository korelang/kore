#include "targets/bytecode/vm/value_type.hpp"

namespace kore {
    namespace vm {
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

        std::ostream& operator<<(std::ostream& out, const Value& value) {
            switch (value.tag) {
                case ValueTag::Bool:
                    out << "Value(" << (value.value._bool == 1 ? "true" : "false") << ", bool)";
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

                /* case ValueTag::Str: */
                /*     out << "Value(" << value.value._str << ", str)"; */
                /*     break; */
            }

            return out;
        }
    }
}
