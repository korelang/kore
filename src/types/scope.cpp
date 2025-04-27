#include "types/scope.hpp"
#include "targets/bytecode/register.hpp"

namespace kore {
    bool ScopeEntry::is_global_scope() const {
        return level == 1;
    }

    ScopeEntry* Scope::find(const std::string& name) {
        auto entry = std::find_if(_map.begin(), _map.end(), [&name](ScopeEntry& _entry) {
            return _entry.identifier->name() == name;
        });

        return entry != _map.end() ? &*entry : nullptr;
    }

    void Scope::add(const ScopeEntry& entry) {
        _map.push_back(entry);
    }

    Function* Scope::function() {
        return _func;
    }

    Scope::iterator Scope::begin() {
        return _map.begin();
    }

    Scope::iterator Scope::end() {
        return _map.end();
    }

    ScopeStack::ScopeStack() {
        // Create top-level scope
        enter();
    }

    ScopeStack::~ScopeStack() {}

    int ScopeStack::depth() const {
        return _scopes.size();
    }

    void ScopeStack::enter() {
        _scopes.emplace_back();
    }

    void ScopeStack::enter_function_scope(Function* func) {
        _scopes.emplace_back();
        _functions.push_back(func);
    }

    void ScopeStack::leave() {
        _scopes.pop_back();
    }

    void ScopeStack::leave_function_scope() {
        _scopes.pop_back();
        _functions.pop_back();
    }

    ScopeEntry* ScopeStack::find(const std::string& name) {
        return find_in_range(name, depth(), 1);
    }

    ScopeEntry* ScopeStack::find_inner(const std::string& name) {
        int inner_scope = depth();

        return find_in_range(name, inner_scope, inner_scope);
    }

    ScopeEntry* ScopeStack::find_enclosing(const std::string& name) {
        return find_in_range(name, depth() - 1, 2);
    }

    Function* ScopeStack::enclosing_function() {
        return _functions.empty() ? nullptr : _functions[_functions.size() - 1];
    }

    void ScopeStack::insert(const Identifier* identifier) {
        insert(identifier, INVALID_REGISTER);
    }

    void ScopeStack::insert(const Identifier* identifier, Reg reg) {
        auto& active_scope = _scopes.back();

        auto entry = ScopeEntry {
            reg,
            static_cast<int>(_scopes.size()),
            identifier,
        };

        active_scope.add(entry);
    }

    bool ScopeStack::is_global_scope() const {
        return depth() == 1;
    }

    void ScopeStack::clear() {
        _scopes.clear();
        enter();
    }

    ScopeStack::iterator ScopeStack::begin() {
        return _scopes.begin();
    }

    ScopeStack::iterator ScopeStack::end() {
        return _scopes.end();
    }

    ScopeStack::iterator ScopeStack::func_begin() {
        return _scopes.begin();
    }

    ScopeStack::iterator ScopeStack::func_end() {
        return _scopes.end();
    }

    ScopeEntry* ScopeStack::find_in_range(const std::string& name, int start_lvl, int end_lvl) {
        for (auto lvl = start_lvl - 1; lvl >= end_lvl - 1; --lvl) {
            auto& scope = _scopes[lvl];
            auto entry = scope.find(name);

            if (entry) {
                return entry;
            }
        }

        return nullptr;
    }
}
