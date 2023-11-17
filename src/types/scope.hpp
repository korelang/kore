#ifndef KORE_SCOPES_HPP
#define KORE_SCOPES_HPP

#include <map>
#include <vector>

#include "ast/expressions/identifier.hpp"
#include "ast/statements/function.hpp"
#include "targets/bytecode/register.hpp"

namespace kore {
    /// An entry for each variable with additional bookkeeping
    struct ScopeEntry {
        Reg reg;
        int level;
        const Identifier* identifier;

        bool is_global_scope() const;
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

    // TODO: Convert to a linked list of scopes instead?

    /// A (lexical) scope keeps track of all variables at each level
    class ScopeStack final {
        public:
            ScopeStack();
            virtual ~ScopeStack();

            int levels() const;
            void enter();
            void enter_function_scope(Function* func);
            void leave();
            void leave_function_scope();
            ScopeEntry* find(const std::string& name);
            ScopeEntry* find_inner(const std::string& name);
            ScopeEntry* find_enclosing(const std::string& name);
            Function* enclosing_function();
            void insert(const Identifier* identifier);
            void insert(const Identifier* identifier, Reg reg);
            bool is_global_scope() const;
            bool in_function_scope() const;
            void clear();

        private:
            int _scope_level = 0;
            std::vector<Scope> _scopes;
            std::vector<Function*> _functions;

            ScopeEntry* find_in_range(
                const std::string& name,
                int start_lvl,
                int end_lvl
            );
    };
}

#endif // KORE_SCOPES_HPP
