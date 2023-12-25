#include "constant_table.hpp"

#include <map>

namespace kore {
    ConstantTable::ConstantTable() {}

    ConstantTable::index_type ConstantTable::add(i32 value) {
        return add(vm::Value::from_i32(value));
    }

    ConstantTable::index_type ConstantTable::add(i64 value) {
        return add(vm::Value::from_i64(value));
    }

    ConstantTable::index_type ConstantTable::add(f32 value) {
        return add(vm::Value::from_f32(value));
    }

    ConstantTable::index_type ConstantTable::add(f64 value) {
        return add(vm::Value::from_f64(value));
    }

    ConstantTable::index_type ConstantTable::add(vm::Value constant) {
        auto result = _table.insert({ constant, _constants.size() });
        auto index = result.first->second;

        if (!result.second) {
            // Insertion did not succeed because constant already exists, return its index
            return index;
        }

        _constants.push_back(constant);
        _tagged_constants[constant.tag].push_back(index);

        return index;
    }

    vm::Value ConstantTable::get(ConstantTable::index_type idx) const {
        return _constants[idx];
    }

    size_t ConstantTable::size() const {
        return _constants.size();
    }

    ConstantTable::const_iterator ConstantTable::cbegin() const {
        return _table.cbegin();
    }

    ConstantTable::const_iterator ConstantTable::cend() const {
        return _table.cend();
    }

    ConstantTable::const_value_iterator ConstantTable::sorted_cbegin() const {
        return _constants.cbegin();
    }

    ConstantTable::const_value_iterator ConstantTable::sorted_cend() const {
        return _constants.cend();
    }

    ConstantTable::const_index_iterator ConstantTable::tagged_cbegin(vm::ValueTag tag) const {
        return _tagged_constants.at(tag).cbegin();
    }

    ConstantTable::const_index_iterator ConstantTable::tagged_cend(vm::ValueTag tag) const {
        return _tagged_constants.at(tag).cend();
    }

    void ConstantTable::clear() {
        _table.clear();
        _constants.clear();
    }
}
