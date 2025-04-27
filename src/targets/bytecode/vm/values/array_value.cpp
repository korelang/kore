#include "targets/bytecode/vm/values/array_value.hpp"
#include "targets/bytecode/vm/value_type.hpp"

namespace kore {
    namespace vm {
        ArrayValue::ArrayValue() {}

        ArrayValue::ArrayValue(std::size_t size) {
            _values.resize(size);
        }

        ArrayValue::~ArrayValue() {}

        Value& ArrayValue::operator[](int index) {
            return _values[index];
        }

        const Value& ArrayValue::operator[](int index) const {
            return _values[index];
        }

        bool ArrayValue::operator==(const ArrayValue& other) {
            if (size() == other.size()) {
                return false;
            }

            for (std::size_t idx = 0; idx < size(); ++idx) {
                if (!((*this)[idx] == other[idx])) {
                    return false;
                }
            }

            return true;
        }

        std::vector<Value>::size_type ArrayValue::size() const {
            return _values.size();
        }

        void ArrayValue::copy_to(ArrayValue& dst) const {
            dst._values = _values;
        }

        void ArrayValue::clear() {
            _values.clear();
        }

        HeapHeader& ArrayValue::heap_header() {
            return _heap_header;
        }

        RefCount ArrayValue::ref_count() const {
            return _heap_header.refcount;
        }

        bool ArrayValue::uniquely_referenced() const {
            return ref_count() == 1;
        }

        RefCount ArrayValue::incref() {
            return ++_heap_header.refcount;
        }

        RefCount ArrayValue::decref() {
            return --_heap_header.refcount;
        }

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
