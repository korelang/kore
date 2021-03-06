#ifndef KORE_SCOPES_HPP
#define KORE_SCOPES_HPP

#include <map>
#include <vector>

#include "ast/expressions/identifier.hpp"
#include "backend/vm/vm.hpp"

namespace kore {
    /// An entry for each variable with additional bookkeeping
    struct ScopeEntry {
        Reg reg;
        int level;
        const Identifier* identifier;
    };

    namespace {
        using ScopeMap = std::map<std::string, ScopeEntry>;

        struct Scope {
            Scope(bool func_scope_start)
                : func_scope_start(func_scope_start) {}

            ScopeEntry* find(const std::string& name);

            bool func_scope_start;
            ScopeMap _map;
        };
    }

    /// A (lexical) scope keeps track of all variables at each level
    class ScopeStack final {
        public:
            ScopeStack();
            virtual ~ScopeStack();

            int levels() const;
            void enter();
            void enter_function_scope();
            void leave();
            ScopeEntry* find(const std::string& name);
            ScopeEntry* find_inner(const std::string& name);
            ScopeEntry* find_enclosing(const std::string& name);
            void insert(const Identifier* identifier);
            void insert(const Identifier* identifier, Reg reg);
            void clear();

        private:
            std::vector<Scope> _scopes;

            ScopeEntry* find_in_range(
                const std::string& name,
                int start_lvl,
                int end_lvl
            );
    };
}

#endif // KORE_SCOPES_HPP
