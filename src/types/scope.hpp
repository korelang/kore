#ifndef KORE_SCOPES_HPP
#define KORE_SCOPES_HPP

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

    /// A single lexical scope including all the variables defined in it
    class Scope {
        private:
            using ScopeMap = std::vector<ScopeEntry>;

        public:
            using iterator = ScopeMap::iterator;

            ScopeEntry* find(const std::string& name);

            void add(const ScopeEntry& entry);

            /// Get the function that contains this scope if any
            Function* function();

            iterator begin();
            iterator end();

        private:
            ScopeMap _map;
            Function* _func = nullptr;
    };

    /// A stack of scopes going from the outermost to the innermost scope
    class ScopeStack final {
        public:
            using iterator = std::vector<Scope>::iterator;

            ScopeStack();
            virtual ~ScopeStack();

            /// The depth of scopes
            int depth() const;

            /// Enter a new scope
            void enter();

            /// Enter a function scope
            void enter_function_scope(Function* func);

            /// Leave a scope
            void leave();

            /// Leave a function scope
            void leave_function_scope();

            /// Find identifiers by name
            ScopeEntry* find(const std::string& name);
            ScopeEntry* find_inner(const std::string& name);
            ScopeEntry* find_enclosing(const std::string& name);

            /// Get the enclosing function, if any, of the current scope
            Function* enclosing_function();

            /// Insert identifiers into the current scope
            void insert(const Identifier* identifier);
            void insert(const Identifier* identifier, Reg reg);

            Scope* current_scope();

            bool is_global_scope() const;
            bool in_function_scope() const;

            /// Clear the scope stack
            void clear();

            iterator begin();
            iterator end();

            // TODO: Make a custom iterator that goes through all scopes except
            // the global scope or outside the current function scope
            iterator func_begin();
            iterator func_end();

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
