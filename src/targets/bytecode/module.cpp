#include "module.hpp"
#include "targets/bytecode/constant_table_tag.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"

namespace kore {
    Module::Module() : Module(0, "") {}

    Module::Module(ModuleIndex idx, const fs::path& path)
        : _idx(idx),
          _path(path) {}

    Module::~Module() {}

    Version Module::get_compiler_version() {
        return _compiler_version;
    }

    Version Module::get_bytecode_version() {
        return _bytecode_version;
    }

    void Module::set_compiler_version(std::array<char, 3> version) {
        _compiler_version.major = version[0];
        _compiler_version.minor = version[1];
        _compiler_version.patch = version[2];
    }

    void Module::set_bytecode_version(std::array<char, 3> version) {
        _bytecode_version.major = version[0];
        _bytecode_version.minor = version[1];
        _bytecode_version.patch = version[2];
    }

    std::string Module::name() const {
        // TODO:
        return "main";
    }

    ModuleIndex Module::index() const {
        return _idx;
    }

    std::string Module::path() const {
        return _path;
    }

    int Module::objects_count() const {
        return _objects.size();
    }

    ConstantTable::index_type Module::add_constant(vm::Value value) {
        return _constants.add(value);
    }

    const ConstantTable& Module::constant_table() const {
        return _constants;
    }

    Module::object_iterator Module::objects_begin() const {
        auto it = _objects.cbegin()++;

        return it;
    }

    Module::object_iterator Module::objects_end() const {
        return _objects.end();
    }

    void Module::set_global_indices_count(int count) {
        _global_indices = count;
    }

    int Module::global_indices_count() const {
        return _global_indices;
    }

    CompiledObject* Module::new_compiled_object() {
        _objects.emplace_back();

        return _objects.back().get();
    }

    CompiledObject* Module::new_function(const Function& func) {
        _objects.emplace_back(std::make_unique<CompiledObject>(
            &func,
            get_free_function_index()
        ));

        std::string name = func.name();
        _function_map[name] = _objects.back().get();

        return _function_map[name];
    }

    CompiledObject* Module::new_function_from_name(const std::string& name) {
        _objects.emplace_back(std::make_unique<CompiledObject>(name, get_free_function_index()));
        _function_map[name] = _objects.back().get();

        return _function_map[name];
    }

    CompiledObject* Module::main_object() {
        return get_function("<main>");
    }

    CompiledObject* Module::get_function(const std::string& name) {
        auto it = _function_map.find(name);

        return it != _function_map.end() ? it->second : nullptr;
    }

    CompiledObject* Module::get_function_by_index(size_t index) {
        if (index <= 0 || index >= _objects.size()) {
            return nullptr;
        }

        return _objects[index].get();
    }

    void Module::add_function(
        const std::string& name,
        int lnum,
        int start,
        int end,
        int func_index,
        int locals_count,
        int reg_count,
        const std::vector<bytecode_type>& instructions
    ) {
        auto location = SourceLocation(lnum, start, end);

        _objects.emplace_back(std::make_unique<CompiledObject>(
            name,
            location,
            func_index,
            locals_count,
            reg_count,
            instructions
        ));

        _function_map[name] = _objects.back().get();
    }

    int Module::get_free_function_index() {
        return Module::_free_func_index++;
    }

    int Module::_free_func_index = 0;
}
