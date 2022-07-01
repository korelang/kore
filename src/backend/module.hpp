#ifndef KORE_MODULE_HPP
#define KORE_MODULE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/* #include "codegen/bytecode/bytecode_codegen.hpp" */
#include "codegen/compiled_object.hpp"
#include "vm/value_type.hpp"

namespace kore {
    /* class BytecodeGenerator; */

    using function_map = std::unordered_map<std::string, CompiledObject::pointer>;

    template<typename T>
    using constant_table = std::unordered_map<T, int>;

    /// A module represents a kore module and is a collection of constants and
    /// compiled code
    class Module final {
        public:
            using pointer = std::unique_ptr<Module>;
            using object_iterator = std::vector<CompiledObject::const_pointer>::const_iterator;

            template<typename T>
            using value_iterator = typename std::vector<T>::const_iterator;

        public:
            Module();
            /* Module(BytecodeGenerator::function_map&& funcs, std::vector<CompiledObject::pointer>&& objects); */
            virtual ~Module();

            std::string path() const noexcept;
            std::size_t objects_count() const;
            std::size_t constants_count() const;

            int add_i32_constant(i32 constant);
            int add_f32_constant(f32 constant);

            object_iterator objects_begin() const;
            object_iterator objects_end() const;
            value_iterator<i32> i32_constants_begin() const;
            value_iterator<i32> i32_constants_end() const;

            CompiledObject* new_compiled_object();
            CompiledObject* new_function(const Function& func);
            CompiledObject* new_function_from_name(const std::string& name);
            CompiledObject* main_object();
            /* const CompiledObject* main_object() const; */
            CompiledObject* get_function(const std::string& name);

        private:
            std::string _path;
            std::vector<CompiledObject::pointer> _objects;

            // Map a function name to its compiled code
            function_map _function_map;

            std::vector<i32> _i32_constants;
            std::vector<f32> _f32_constants;

            // A mapping from values to their index in the constant table
            constant_table<i32> _i32_constant_table;
            constant_table<f32> _f32_constant_table;
    };
}

#endif // KORE_MODULE_HPP
