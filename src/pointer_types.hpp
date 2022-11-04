#ifndef KORE_POINTER_TYPES_HPP
#define KORE_POINTER_TYPES_HPP

#include <memory>

namespace kore {
    template<typename T, typename Deleter = std::default_delete<T>>
    using Owned = std::unique_ptr<T, Deleter>;

    template<typename T>
    using Shared = std::shared_ptr<T>;
}

#endif // KORE_POINTER_TYPES_HPP
