#include "module.hpp"

namespace kore {
    Module::Module()
        : _i32_constants(ConstantTableTag::I32),
          _i64_constants(ConstantTableTag::I64),
          _f32_constants(ConstantTableTag::F32),
          _f64_constants(ConstantTableTag::F64),
          _str_constants(ConstantTableTag::Str) {}

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
        // TODO
        return "main";
    }

    std::string Module::path() const {
        return _path;
    }

    int Module::objects_count() const {
        return _objects.size();
    }

    int Module::constants_count() const {
        return _i32_constants.size() + _f32_constants.size() + _i64_constants.size() + _f64_constants.size();
    }

    int Module::add_i32_constant(i32 constant) {
        return _i32_constants.add(constant);
    }

    int Module::add_i64_constant(i64 constant) {
        return _i64_constants.add(constant);
    }

    int Module::add_f32_constant(f32 constant) {
        return _f32_constants.add(constant);
    }

    int Module::add_f64_constant(f64 constant) {
        return _f64_constants.add(constant);
    }

    Module::object_iterator Module::objects_begin() const {
        auto it = _objects.cbegin()++;

        return it;
    }

    Module::object_iterator Module::objects_end() const {
        return _objects.end();
    }

    const ConstantTable<i32>& Module::i32_constant_table() const {
        return _i32_constants;
    }

    const ConstantTable<i64>& Module::i64_constant_table() const {
        return _i64_constants;
    }

    const ConstantTable<f32>& Module::f32_constant_table() const {
        return _f32_constants;
    }

    const ConstantTable<f64>& Module::f64_constant_table() const {
        return _f64_constants;
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

        _objects.emplace_back(std::make_unique<CompiledObject>(name, location, func_index, locals_count, reg_count, instructions));
        _function_map[name] = _objects.back().get();
    }

    int Module::get_free_function_index() {
        return Module::_free_func_index++;
    }

    int Module::_free_func_index = 0;
}
