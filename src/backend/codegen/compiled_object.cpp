#include <algorithm>

#include "compiled_object.hpp"
#include "vm/vm.hpp"

namespace kore {
    CompiledObject::CompiledObject() {}

    CompiledObject::CompiledObject(const std::string& name) : _name(name) {}

    CompiledObject::CompiledObject(const Function* func) {
        _name = func->name();
        _location = func->location();
    }

    CompiledObject::CompiledObject(
        const std::string& name,
        const Location& location,
        int locals_count,
        int reg_count,
        const std::vector<bytecode_type>& instructions
    ) : _name(name),
        _location(location),
        _local_count(locals_count),
        _instructions(instructions),
        _reg_count(reg_count),
        _max_regs_used(0) {}

    CompiledObject::~CompiledObject() {}

    std::string CompiledObject::name() const {
        return _name;
    }

    Location CompiledObject::location() const {
        return _location;
    }

    int CompiledObject::locals_count() const {
        return _local_count;
    }

    int CompiledObject::reg_count() const {
        return _reg_count;
    }

    int CompiledObject::max_regs_used() const {
        return _max_regs_used;
    }

    Reg CompiledObject::allocate_register() {
        if (_reg_count >= KORE_VM_MAX_REGISTERS) {
            throw std::runtime_error("register overflow");
        }

        _max_regs_used = std::max(_max_regs_used, _reg_count + 1);

        return _reg_count++;
    }

    void CompiledObject::free_registers(int count) {
        _reg_count -= count;
    }

    void CompiledObject::add_instruction(bytecode_type instruction) {
        _instructions.push_back(instruction);
    }

    int CompiledObject::code_size() const {
        return _instructions.size();
    }

    bool CompiledObject::is_main_object() const {
        return name() == "<main>";
    }

    bytecode_type& CompiledObject::operator[](int index) {
        return _instructions[index];
    }

    const bytecode_type& CompiledObject::operator[](int index) const {
        return _instructions[index];
    }

    CompiledObject::instruction_iterator CompiledObject::begin() const {
        return _instructions.begin();
    }

    CompiledObject::instruction_iterator CompiledObject::end() const {
        return _instructions.end();
    }

    const bytecode_type* CompiledObject::instructions() const {
        return _instructions.data();
    }
}
