#ifndef KORE_HEAP_HEADER_HPP
#define KORE_HEAP_HEADER_HPP

namespace kore {
    namespace vm {
        using RefCount = unsigned int;

        struct HeapHeader {
            RefCount refcount;
        };
    }
}

#endif // KORE_HEAP_HEADER_HPP
