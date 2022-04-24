#include "compiled_object.hpp"
#include "vm/vm.hpp"

namespace kore {
    CompiledObject::CompiledObject() {}

    CompiledObject::CompiledObject(const std::string& name) : _name(name) {}

    CompiledObject::CompiledObject(Function* func) {
        if (func) {
            _name = func->name();
            _location = func->location();
        }
    }

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

    Reg CompiledObject::allocate_register() {
        if (_reg_count >= KORE_VM_MAX_REGISTERS) {
            throw std::runtime_error("No more registers");
        }

        return _reg_count++;
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

    CompiledObject::instruction_iterator CompiledObject::begin() const {
        return _instructions.begin();
    }

    CompiledObject::instruction_iterator CompiledObject::end() const {
        return _instructions.end();
    }

    bytecode_type* CompiledObject::instructions() {
        return _instructions.data();
    }
}
