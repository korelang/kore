#include "ast/statements/statements.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/kir.hpp"

namespace kore {
    namespace kir {
        Kir::Kir() {}
        Kir::~Kir() {}

        int Kir::globals_count() {
            return 0;
        }

        /* const ConstantTable<i32>& Kir::i32_constant_table() { */
        /*     return _i32_table; */
        /* } */

        void Kir::add_module(Module& module) {
            _modules.push_back(module);
        }

        Module& Kir::main_module() {
            return _modules[_main_module_index];
        }

        void Kir::set_main_module(ModuleIndex idx) {
            _main_module_index = idx;
        }

        std::size_t Kir::module_count() const {
            return _modules.size();
        }

        Kir::iterator Kir::begin() {
            return _modules.begin();
        }

        Kir::iterator Kir::end() {
            return _modules.end();
        }

        Kir::const_iterator Kir::cbegin() const {
            return _modules.cbegin();
        }

        Kir::const_iterator Kir::cend() const {
            return _modules.cend();
        }
    }
}
