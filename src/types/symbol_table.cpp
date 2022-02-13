#include "types/symbol_table.hpp"

namespace kore {
    SymbolTable::SymbolTable() {}

    SymbolTable::~SymbolTable() {}

    const Identifier* SymbolTable::find_identifier(const std::string& name) {
        auto it = _identifiers.find(name);

        if (it != _identifiers.end()) {
            return it->second;
        }

        return nullptr;
    }

    bool SymbolTable::find_identifier(Identifier* identifier) {
        return _identifiers.find(identifier->name()) != _identifiers.end();
    }

    bool SymbolTable::insert_identifier(const Identifier* identifier) {
        auto result = _identifiers.insert({identifier->name(), identifier});

        return result.second;
    }
}
