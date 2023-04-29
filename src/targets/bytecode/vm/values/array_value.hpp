#ifndef KORE_ARRAY_VALUE_HPP
#define KORE_ARRAY_VALUE_HPP

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
                ~ArrayValue();

                static ArrayValue* allocate(std::size_t size);

            private:
                std::vector<Value> _values;
        };

        std::ostream& operator<<(std::ostream& out, const ArrayValue& value);
    }
}

#endif // KORE_ARRAY_VALUE_HPP
