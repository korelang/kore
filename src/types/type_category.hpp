#ifndef KORE_TYPE_CATEGORY_HPP
#define KORE_TYPE_CATEGORY_HPP

namespace kore {
    // Use this approach or make subclasses?
    enum class TypeCategory {
        Array,
        Bool,
        Byte,
        Char,
        Float32,
        Float64,
        Function,
        Generic,
        Integer32,
        Integer64,
        Map,
        Optional,
        Str,
        Struct,
        Unknown,
        Void,
    };
}

#endif // KORE_TYPE_CATEGORY_HPP
