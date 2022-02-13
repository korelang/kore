#ifndef KORE_SYMBOL_TABLE_HPP
#define KORE_SYMBOL_TABLE_HPP

#include <unordered_map>

#include "ast/expressions/identifier.hpp"

namespace kore {
    /// Keeps track of the variables in the program and their scope
    class SymbolTable final {
        public:
            SymbolTable();
            ~SymbolTable();

            const Identifier* find_identifier(const std::string& name);
            bool find_identifier(Identifier* identifier);
            bool insert_identifier(const Identifier* identifier);

        private:
            std::unordered_map<std::string, const Identifier*> _identifiers;
    };
}

#endif // KORE_SYMBOL_TABLE_HPP
