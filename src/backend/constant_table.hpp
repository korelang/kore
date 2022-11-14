#ifndef KORE_CONSTANT_TABLE_HPP
#define KORE_CONSTANT_TABLE_HPP

#include <unordered_map>

#include "constant_table_tag.hpp"

namespace kore {
    template<typename T, typename IndexType = int>
    class ConstantTable final {
        public:
            using const_iterator = typename std::unordered_map<T, IndexType>::const_iterator;

        public:
            ConstantTable(ConstantTableTag tag) : _tag(tag) {}

            ConstantTableTag tag() const {
                return _tag;
            }

            IndexType add(T constant) {
                auto it = _table.find(constant);

                if (it != _table.end()) {
                    return it->second;
                }

                _constants.push_back(constant);

                return _table.insert({ constant, _constants.size() - 1 }).first->second;
            }

            T get(IndexType idx) const {
                return _constants[idx];
            }

            size_t size() const {
                return _constants.size();
            }

            const_iterator cbegin() const {
                return _table.cbegin();
            }

            const_iterator cend() const {
                return _table.cend();
            }

            void clear() {
                _table.clear();
                _constants.clear();
            }

        private:
            ConstantTableTag _tag;
            std::unordered_map<T, IndexType> _table;
            std::vector<T> _constants;
    };
}

#endif // KORE_CONSTANT_TABLE_HPP
