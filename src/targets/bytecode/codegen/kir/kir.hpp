#ifndef KORE_KIR_HPP
#define KORE_KIR_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "errors/errors.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/module.hpp"
#include "types/scope.hpp"

namespace kore {
    namespace kir {
        /// A program represented in KIR form (kore intermediate representation)
        class Kir final {
            public:
                using iterator = std::vector<Module>::iterator;
                using const_iterator = std::vector<Module>::const_iterator;

            public:
                Kir();
                virtual ~Kir();

                int globals_count();
                const ConstantTable<i32>& i32_constant_table();
                void add_module(Module& module);
                Module& main_module();
                void set_main_module(ModuleIndex idx);
                std::size_t module_count() const;

                iterator begin();
                iterator end();
                const_iterator cbegin() const;
                const_iterator cend() const;

            private:
                ModuleIndex _main_module_index;
                std::vector<Module> _modules;
                /* ConstantTable<i32> _i32_table; */
        };
    }
}

#endif // KORE_KIR_HPP

