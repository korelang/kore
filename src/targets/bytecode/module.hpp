#ifndef KORE_MODULE_HPP
#define KORE_MODULE_HPP

#include <array>
#include <filesystem>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "pointer_types.hpp"
#include "version.hpp"
#include "targets/bytecode/compiled_object.hpp"
#include "targets/bytecode/constant_table.hpp"
#include "targets/bytecode/vm/value_type.hpp"

namespace fs = std::filesystem;

namespace kore {
    using ModuleIndex = unsigned int;
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
            Module(ModuleIndex idx, const fs::path& path);
            virtual ~Module();

            Version get_compiler_version();
            Version get_bytecode_version();
            void set_compiler_version(std::array<char, 3> version);
            void set_bytecode_version(std::array<char, 3> version);

            std::string name() const;
            ModuleIndex index() const;
            std::string path() const;
            int objects_count() const;

            ConstantTable::index_type add_constant(vm::Value value);
            const ConstantTable& constant_table() const;

            object_iterator objects_begin() const;
            object_iterator objects_end() const;

            void set_global_indices_count(int count);
            int global_indices_count() const;

            CompiledObject* new_compiled_object();
            CompiledObject* new_function(const Function& func);
            CompiledObject* new_function_from_name(const std::string& name);
            CompiledObject* main_object();
            /* const CompiledObject* main_object() const; */
            CompiledObject* get_function(const std::string& name);
            CompiledObject* get_function_by_index(int index);
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

            ModuleIndex _idx;
            fs::path _path;
            std::vector<CompiledObject::pointer> _objects;

            // Map a function name to its compiled code
            function_map _function_map;

            ConstantTable _constants;

            int _global_indices;

            // TODO: Probably move this into CompiledObject
            static int get_free_function_index();

            static int _free_func_index;
    };
}

#endif // KORE_MODULE_HPP
