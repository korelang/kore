#ifndef KORE_MODULE_HPP
#define KORE_MODULE_HPP

#include <array>
#include <unordered_map>
#include <vector>

#include "codegen/compiled_object.hpp"
#include "constant_table.hpp"
#include "pointer_types.hpp"
#include "version.hpp"
#include "vm/value_type.hpp"

namespace kore {
    using function_map = std::unordered_map<std::string, CompiledObject*>;

    /// A module represents a kore module and is a collection of constants and
    /// compiled code
    class Module final {
        public:
            using pointer = Owned<Module>;
            using object_iterator = std::vector<CompiledObject::const_pointer>::const_iterator;

        public:
            Module();
            Module(Module&& module) = default;
            virtual ~Module();

            Version get_compiler_version();
            Version get_bytecode_version();
            void set_compiler_version(std::array<char, 3> version);
            void set_bytecode_version(std::array<char, 3> version);

            std::string name() const;
            std::string path() const;
            int objects_count() const;
            int constants_count() const;

            int add_i32_constant(i32 constant);
            int add_f32_constant(f32 constant);
            int add_i64_constant(i64 constant);
            int add_f64_constant(f64 constant);

            object_iterator objects_begin() const;
            object_iterator objects_end() const;

            const ConstantTable<i32>& i32_constant_table() const;
            const ConstantTable<i64>& i64_constant_table() const;
            const ConstantTable<f32>& f32_constant_table() const;
            const ConstantTable<f64>& f64_constant_table() const;

            void set_global_indices_count(int count);
            int global_indices_count() const;

            CompiledObject* new_compiled_object();
            CompiledObject* new_function(const Function& func);
            CompiledObject* new_function_from_name(const std::string& name);
            CompiledObject* main_object();
            /* const CompiledObject* main_object() const; */
            CompiledObject* get_function(const std::string& name);
            void add_function(
                const std::string& name,
                int lnum,
                int start,
                int end,
                int func_index,
                int locals_count,
                int reg_count,
                const std::vector<bytecode_type>& instructions
            );

        private:
            // The version of the compiler used to compile this module
            Version _compiler_version;

            // The version of the bytecode format in this module
            Version _bytecode_version;

            std::string _path;
            std::vector<CompiledObject::pointer> _objects;

            // Map a function name to its compiled code
            function_map _function_map;

            // Constant tables
            ConstantTable<i32> _i32_constants;
            ConstantTable<i64> _i64_constants;
            ConstantTable<f32> _f32_constants;
            ConstantTable<f64> _f64_constants;
            ConstantTable<std::string> _str_constants;

            int _global_indices;

            // TODO: Probably move this into CompiledObject
            static int get_free_function_index();

            static int _free_func_index;
    };
}

#endif // KORE_MODULE_HPP
