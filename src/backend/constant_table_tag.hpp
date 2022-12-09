#ifndef KORE_CONSTANT_TABLE_TAG_HPP
#define KORE_CONSTANT_TABLE_TAG_HPP

namespace kore {
    // Tags to identify entries in the constant table
    enum class ConstantTableTag {
        I32 = 0,
        I64,
        F32,
        F64,
        Str
    };
}

#endif // KORE_CONSTANT_TABLE_TAG_HPP
