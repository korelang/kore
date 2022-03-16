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

            bool func_scope_start;

            ScopeMap _map;
        };
    }

    /// A (lexical) scope keeps track of all variables at each level
    class ScopeStack final {
        public:
            ScopeStack();
            virtual ~ScopeStack();

            void enter(bool func_scope_start = false);
            void leave();
            ScopeEntry* get(const std::string& name);
            void insert(const Identifier* identifier);
            void clear();

        private:
            std::vector<Scope> _scopes;
    };
}

#endif // KORE_SCOPES_HPP
