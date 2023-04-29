#include "targets/bytecode/vm/values/array_value.hpp"
#include "targets/bytecode/vm/value_type.hpp"

namespace kore {
    namespace vm {
        ArrayValue::ArrayValue() {}
        ArrayValue::~ArrayValue() {}

        ArrayValue* ArrayValue::allocate(std::size_t size) {
            ArrayValue* array = new ArrayValue();

            if (!array) {
                throw std::runtime_error("Out of memory");
            }

            array->_values.resize(size);

            return array;
        }

        std::ostream& operator<<(std::ostream& out, const ArrayValue& value) {
            out << "[";
            auto values = value._values;

            for (std::size_t idx = 0; idx < values.size(); ++idx) {
                out << values[idx];

                if (idx < values.size() - 1) {
                    out << ", ";
                }
            }

            out << "]";

            return out;
        }
    }
}
