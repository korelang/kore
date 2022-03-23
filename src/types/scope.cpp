#include "types/scope.hpp"

namespace kore {
    ScopeEntry* Scope::find(const std::string& name) {
        auto entry = _map.find(name);

        return entry != _map.end() ? &entry->second : nullptr;
    }

    ScopeStack::ScopeStack() {
        // Create top-level scope
        enter(false);
    }

    ScopeStack::~ScopeStack() {}

    int ScopeStack::levels() const {
        return _scopes.size();
    }

    void ScopeStack::enter(bool func_scope_start) {
        _scopes.emplace_back(func_scope_start);
    }

    void ScopeStack::leave() {
        _scopes.pop_back();
    }

    ScopeEntry* ScopeStack::find(const std::string& name) {
        return find_in_range(name, levels(), 1);
    }

    ScopeEntry* ScopeStack::find_inner(const std::string& name) {
        int inner_scope = levels();

        return find_in_range(name, inner_scope, inner_scope);
    }

    ScopeEntry* ScopeStack::find_enclosing(const std::string& name) {
        return find_in_range(name, levels() - 1, 1);
    }

    void ScopeStack::insert(const Identifier* identifier) {
        insert(identifier, 0);
    }

    void ScopeStack::insert(const Identifier* identifier, Reg reg) {
        auto& last = _scopes.back()._map;

        last.emplace(
            identifier->name(),
            ScopeEntry {
                .reg = reg,
                .level = static_cast<int>(_scopes.size()),
                .identifier = identifier,
            }
        );
    }

    void ScopeStack::clear() {
        _scopes.clear();
    }

    ScopeEntry* ScopeStack::find_in_range(
        const std::string& name,
        int start_lvl,
        int end_lvl
    ) {
        for (auto lvl = start_lvl - 1; lvl >= end_lvl - 1; --lvl) {
            auto& scope = _scopes[lvl];
            auto entry = scope.find(name);

            if (entry) {
                return entry;
            }

            if (scope.func_scope_start) {
                // This scope was entered when entering a function, so don't
                // start searching the function's outer scopes
                return nullptr;
            }
        }

        return nullptr;
    }
}
