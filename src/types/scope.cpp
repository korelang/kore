#include "types/scope.hpp"

namespace kore {
    ScopeStack::ScopeStack() {
        // Create top-level scope
        enter(false);
    }

    ScopeStack::~ScopeStack() {}

    void ScopeStack::enter(bool func_scope_start) {
        _scopes.emplace_back(func_scope_start);
    }

    void ScopeStack::leave() {
        _scopes.pop_back();
    }

    ScopeEntry* ScopeStack::get(const std::string& name) {
        // Examine the scopes in reverse order i.e. starting with the
        // inner-most scope
        for (auto it = _scopes.rbegin(); it != _scopes.rend(); ++it) {
            auto& scope = *it;
            auto entry = scope._map.find(name);

            if (entry != scope._map.end()) {
                return &entry->second;
            }

            if (scope.func_scope_start) {
                // This scope was entered when entering a function, so don't
                // start searching the function's outer scopes
                return nullptr;
            }
        }

        return nullptr;
    }

    void ScopeStack::insert(const Identifier* identifier) {
        auto& last = _scopes.back()._map;

        last.emplace(
            identifier->name(),
            ScopeEntry {
                .reg = 0,
                .level = static_cast<int>(_scopes.size()),
                .identifier = identifier,
            }
        );
    }

    void ScopeStack::clear() {
        _scopes.clear();
    }
}
