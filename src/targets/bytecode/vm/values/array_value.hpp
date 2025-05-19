#ifndef KORE_ARRAY_VALUE_HPP
#define KORE_ARRAY_VALUE_HPP

#include "targets/bytecode/vm/heap_header.hpp"
#include <ostream>
#include <vector>

namespace kore {
    namespace vm {
        struct Value;

        class ArrayValue final {
            public:
                friend std::ostream& operator<<(
                    std::ostream& out,
                    const ArrayValue& value
                );

            public:
                ArrayValue();
                ArrayValue(std::size_t size);
                ~ArrayValue();

                Value& operator[](int index);
                const Value& operator[](int index) const;
                bool operator==(const ArrayValue& other);
                std::vector<Value>::size_type size() const;
                void copy_to(ArrayValue& dst) const;
                void clear();

                HeapHeader& heap_header();
                RefCount ref_count() const;
                bool uniquely_referenced() const;
                RefCount incref();
                RefCount decref();

                static ArrayValue* allocate(std::size_t size);

            private:
                std::vector<Value> _values;
                HeapHeader _heap_header;
        };

        std::ostream& operator<<(std::ostream& out, const ArrayValue& value);
    }
}

#endif // KORE_ARRAY_VALUE_HPP
