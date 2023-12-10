#include "function_value.hpp"

namespace kore {
    namespace vm {
        std::ostream& operator<<(std::ostream& out, const FunctionValue& value) {
            return out
                << "<func "
                << "\"" << value.obj->name() << "\" "
                << "(" << value.index << ")>";
        }
    }
}
