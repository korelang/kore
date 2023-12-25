#ifndef KORE_KIR_MODULE_HPP
#define KORE_KIR_MODULE_HPP

#include "ast/statements/function.hpp"
#include "targets/bytecode/constant_table.hpp"
#include "targets/bytecode/module.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace kore {
    namespace kir {
        /// A KIR module consisting of functions and constant tables
        class Module final {
            public:
                using iterator = std::vector<Function>::iterator;
                using const_iterator = std::vector<Function>::const_iterator;

            public:
                Module(ModuleIndex index, const fs::path& path);
                virtual ~Module();

                std::string path() const;
                ModuleIndex index() const noexcept;

                int add_function(const kore::Function* function);
                Function& operator[](int index);
                const Function& operator[](int index) const;
                Function& main_function();

                int function_count() const noexcept;
                int global_count() const noexcept;

                iterator begin();
                iterator end();
                const_iterator cbegin() const;
                const_iterator cend() const;

                ConstantTable& constant_table();
                const ConstantTable& constant_table() const;

            private:
                ModuleIndex _index;
                std::string _name;
                std::string _path;

                int _main_function_index;
                std::vector<Function> _functions;
                int _global_indices;
                ConstantTable _constants;
        };
    }
}

#endif // KORE_KIR_MODULE_HPP
