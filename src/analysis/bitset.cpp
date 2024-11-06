#include <cmath>

#include "analysis/bitset.hpp"

namespace kore {
    bitset_index_proxy::bitset_index_proxy(bitset& bs, size_t idx, int shift)
        : bs(bs), idx(idx), shift(shift) {}

    void bitset_index_proxy::operator=(int value) {
        bs._bits[idx] |= value << shift;
    }

    bitset::bitset(size_t capacity) {
        _bits.reserve(std::ceil(capacity / ELEMENT_SIZE));
    }

    bitset::reference_type bitset::operator[](int idx) {
        return bitset_index_proxy{
            *this,
            idx / ELEMENT_SIZE,
            static_cast<int>(idx % ELEMENT_SIZE),
        };
    }

    bitset bitset::operator+(const bitset& other) const {
        auto bs = *this;

        bs += other;

        return bs;
    }

    bitset& bitset::operator+=(const bitset& other) {
        if (_bits.size() < other._bits.size()) {
            _bits.resize(other._bits.size() - _bits.size());
        }

        for (size_t idx = 0; idx < _bits.size(); ++idx) {
            _bits[idx] |= other._bits[idx];
        }

        return *this;
    }

    bitset bitset::operator-(const bitset& other) const {
        auto bs = *this;

        bs -= other;

        return bs;
    }

    bitset& bitset::operator-=(const bitset& other) {
        if (_bits.size() < other._bits.size()) {
            _bits.resize(other._bits.size() - _bits.size());
        }

        for (size_t idx = 0; idx < _bits.size(); ++idx) {
            _bits[idx] ^= other._bits[idx];
        }

        return *this;
    }

    /* size_t size() const { */
    /*     return _size; */
    /* } */

    /* size_t capacity() const { */
    /*     return _bits.size() * ELEMENT_SIZE; */
    /* } */
}
