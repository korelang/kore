#ifndef KORE_KIR_MODULE_HPP
#define KORE_KIR_MODULE_HPP

#include <filesystem>

#include "ast/statements/function.hpp"
#include "targets/bytecode/constant_table.hpp"
#include "targets/bytecode/module.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"

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

                int add_function();
                int add_function(const kore::Function* function);
                Function& operator[](int index);
                const Function& operator[](int index) const;
                Function& main_function();

                int function_count() const noexcept;
                int constants_count() const noexcept;
                int global_count() const noexcept;

                iterator begin();
                iterator end();
                const_iterator cbegin() const;
                const_iterator cend() const;

                ConstantTable<i32>& i32_constant_table();
                ConstantTable<i64>& i64_constant_table();
                ConstantTable<f32>& f32_constant_table();
                ConstantTable<f64>& f64_constant_table();
                ConstantTable<std::string>& str_constant_table();

                const ConstantTable<i32>& i32_constant_table() const;
                const ConstantTable<i64>& i64_constant_table() const;
                const ConstantTable<f32>& f32_constant_table() const;
                const ConstantTable<f64>& f64_constant_table() const;
                const ConstantTable<std::string>& str_constant_table() const;

            private:
                ModuleIndex _index;
                std::string _name;
                std::string _path;

                std::vector<Function> _functions;
                int _global_indices;
                ConstantTable<i32> _i32_constants;
                ConstantTable<i64> _i64_constants;
                ConstantTable<f32> _f32_constants;
                ConstantTable<f64> _f64_constants;
                ConstantTable<std::string> _str_constants;
        };
    }
}

#endif // KORE_KIR_MODULE_HPP
