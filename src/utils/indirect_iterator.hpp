#ifndef KORE_INDIRECT_ITERATOR_HPP
#define KORE_INDIRECT_ITERATOR_HPP

// Inspired by: https://jonasdevlieghere.com/containers-of-unique-pointers/

namespace kore {
    /// An iterator over an existing iterator that does single-level
    /// indirection on the elements of the underlying iterator but only
    /// exposes const pointers to prohibit modifying the values. Primarily
    /// used to expose containers of smart pointers.
    /// 
    /// This is basically Boost's indirect_iterator
    template<typename base_iterator>
    class indirect_iterator : public base_iterator {
        public:
            using value_type = typename base_iterator::value_type::element_type;
            using pointer = value_type*;
            using reference = value_type&;

            indirect_iterator(const base_iterator &it) : it(it) {}

            reference operator*() const {
                return *base_iterator::operator*();
            }

            pointer operator->() {
                return base_iterator::operator*().get();
            }

            const pointer operator->() const {
                return base_iterator::operator*().get();
            }

        private:
            base_iterator it;
    };
}

#endif // KORE_INDIRECT_ITERATOR_HPP
