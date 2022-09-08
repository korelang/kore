#include "module.hpp"

namespace kore {
    Module::Module() {}

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

    std::string Module::path() const noexcept {
        return _path;
    }

    int Module::objects_count() const {
        return _objects.size();
    }

    int Module::constants_count() const {
        return _i32_constants.size() + _f32_constants.size();
    }

    int Module::add_i32_constant(i32 constant) {
        auto it = _i32_constant_table.find(constant);

        if (it != _i32_constant_table.end()) {
            return it->second;
        }

        _i32_constants.push_back(constant);

        return _i32_constant_table.insert({ constant, _i32_constants.size() - 1 }).second;
    }

    int Module::add_f32_constant(f32 constant) {
        auto it = _f32_constant_table.find(constant);

        if (it != _f32_constant_table.end()) {
            return it->second;
        }

        _f32_constants.push_back(constant);

        return _f32_constant_table.insert({ constant, _f32_constant_table.size() - 1 }).second;
    }

    Module::object_iterator Module::objects_begin() const {
        auto it = _objects.cbegin()++;

        return it;
    }

    Module::object_iterator Module::objects_end() const {
        return _objects.end();
    }

    Module::value_iterator<i32> Module::i32_constants_begin() const {
        return _i32_constants.cbegin();
    }

    Module::value_iterator<i32> Module::i32_constants_end() const {
        return _i32_constants.cend();
    }

    CompiledObject* Module::new_compiled_object() {
        _objects.emplace_back();

        return _objects.back().get();
    }

    CompiledObject* Module::new_function(const Function& func) {
        _objects.emplace_back(std::make_unique<CompiledObject>(&func));

        std::string name = func.name();
        _function_map[name] = _objects.back().get();

        return _function_map[name];
    }

    CompiledObject* Module::new_function_from_name(const std::string& name) {
        _objects.emplace_back(std::make_unique<CompiledObject>(name));
        _function_map[name] = _objects.back().get();
        /* _function_map[name] = std::make_unique<CompiledObject>(name); */

        return _function_map[name];
    }

    CompiledObject* Module::main_object() {
        return get_function("<main>");
    }

    /* const CompiledObject* Module::main_object() const { */
    /*     if (_function_map.empty()) { */
    /*         return nullptr; */
    /*     } */

    /*     return _function_map["<main>"].get(); */
    /* } */

    CompiledObject* Module::get_function(const std::string& name) {
        auto it = _function_map.find(name);

        return it != _function_map.end() ? it->second : nullptr;
    }

    void Module::add_function(
        const std::string& name,
        int lnum,
        int start,
        int end,
        int locals_count,
        int reg_count,
        const std::vector<bytecode_type>& instructions
    ) {
        auto location = Location(lnum, start, end);

        _objects.emplace_back(std::make_unique<CompiledObject>(name, location, locals_count, reg_count, instructions));
        _function_map[name] = _objects.back().get();
    }
}
