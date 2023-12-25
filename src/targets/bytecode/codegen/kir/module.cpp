#include "targets/bytecode/codegen/kir/module.hpp"

namespace kore {
    namespace kir {
        Module::Module(
            ModuleIndex index,
            const fs::path& path
        ) : _index(index),
            _path(path),
            _global_indices(0)
        {}

        Module::~Module() {}

        std::string Module::path() const {
            return _path;
        }

        ModuleIndex Module::index() const noexcept {
            return _index;
        }

        int Module::add_function(const kore::Function* function) {
            auto func_index = _functions.size();
            _functions.push_back(Function(func_index, function));

            if (!function) {
                _main_function_index = func_index;
            }

            return func_index;
        }

        Function& Module::operator[](int index) {
            return _functions.at(index);
        }

        const Function& Module::operator[](int index) const {
            return _functions.at(index);
        }

        Function& Module::main_function() {
            return _functions[_main_function_index];
        }

        int Module::function_count() const noexcept {
            return _functions.size();
        }

        int Module::global_count() const noexcept {
            return _global_indices;
        }

        Module::iterator Module::begin() {
            return _functions.begin();
        }

        Module::iterator Module::end() {
            return _functions.end();
        }

        Module::const_iterator Module::cbegin() const {
            return _functions.cbegin();
        }

        Module::const_iterator Module::cend() const {
            return _functions.cend();
        }

        ConstantTable& Module::constant_table() {
            return _constants;
        }

        const ConstantTable& Module::constant_table() const {
            return _constants;
        }
    }
}
