#ifndef KORE_CONSTANT_TABLE_HPP
#define KORE_CONSTANT_TABLE_HPP

#include "constant_table_tag.hpp"
#include "targets/bytecode/vm/value_type.hpp"

#include <unordered_map>

namespace kore {
    class ConstantTable final {
        public:
            using index_type = int;
            using const_iterator = typename std::unordered_map<vm::Value, index_type>::const_iterator;
            using const_value_iterator = typename std::vector<vm::Value>::const_iterator;
            using const_index_iterator = typename std::vector<index_type>::const_iterator;

        public:
            ConstantTable();

            index_type add(i32 value);
            index_type add(i64 value);
            index_type add(f32 value);
            index_type add(f64 value);
            index_type add(vm::Value constant);
            vm::Value get(index_type idx) const;
            size_t size() const;
            void clear();

            const_iterator cbegin() const;
            const_iterator cend() const;
            const_value_iterator sorted_cbegin() const;
            const_value_iterator sorted_cend() const;
            const_index_iterator tagged_cbegin(vm::ValueTag tag) const;
            const_index_iterator tagged_cend(vm::ValueTag tag) const;

        private:
            std::unordered_map<vm::Value, index_type> _table;
            std::vector<vm::Value> _constants;
            std::unordered_map<vm::ValueTag, std::vector<index_type>> _tagged_constants;
    };
}

#endif // KORE_CONSTANT_TABLE_HPP
