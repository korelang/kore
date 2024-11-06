#ifndef KORE_BITSET_HPP
#define KORE_BITSET_HPP

#include <cstddef>
#include <vector>

namespace kore {
    class bitset;

    // Like some implementations of std::vector<bool> we have to return a proxy
    // since we cannot return a reference to a single bit
    struct bitset_index_proxy {
        bitset_index_proxy(bitset& bs, size_t idx, int shift);
        void operator=(int value);

        private:
            bitset& bs;
            size_t idx;
            int shift;
    };

    class bitset {
        public: 
            friend bitset_index_proxy;

            using value_type = int;
            using reference_type = bitset_index_proxy;

        public: 
            bitset();
            bitset(size_t size);

            reference_type operator[](int idx);
            /* bitset& operator=(const bitset& other); */

            // Addition means set union
            bitset operator+(const bitset& other) const;
            bitset& operator+=(const bitset& other);

            // Subtraction means set intersection
            bitset operator-(const bitset& other) const;
            bitset& operator-=(const bitset& other);

            /* iterator begin(); */
            /* iterator end(); */

            /* const_iterator cbegin() const; */
            /* const_iterator cend() const; */

            /* size_t size() const; */
            /* size_t capacity() const; */

        private: 
            static const size_t ELEMENT_SIZE = sizeof (std::uint32_t) * 8;

            std::vector<std::uint32_t> _bits;
    };
}

#endif // KORE_BITSET_HPP
