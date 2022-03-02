#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    const std::string BytecodeGenerator::_bytecode_version = "1.0.0";

    BytecodeGenerator::BytecodeGenerator() {}

    BytecodeGenerator::~BytecodeGenerator() {}

    void BytecodeGenerator::emit_version() {
        /* write(_bytecode_version); */
    }

    void BytecodeGenerator::emit_header() {
    }

    void BytecodeGenerator::emit(Bytecode bytecode) {
        _writer->write(bytecode);
        /* auto value = static_cast<bytecode_type>(bytecode); */

        /* char bytes[4] = { */
        /*     static_cast<char>(value >> 24), */
        /*     static_cast<char>(value >> 16), */
        /*     static_cast<char>(value >> 8), */
        /*     static_cast<char>(value & 0xff), */
        /* }; */

        /* _writer->write(&bytes[0], 4); */
    }

    /* void BytecodeGenerator::write(Bytecode code) { */
    /*     *out << code; */
    /* } */

    void BytecodeGenerator::visit(BinaryExpression* expr) {
        switch (expr->type()->category()) {
            /* case TypeCategory::Char: */
            /*     emit(Bytecode::CharAdd); */
            /*     break; */

            case TypeCategory::Float: {
                int reg2 = get_register_operand();
                int reg1 = get_register_operand();
                int dest_reg = allocate_register();
                _writer->write(Bytecode::AddF32, dest_reg, reg1, reg2);
                _register_stack.push(dest_reg);
                break;
            }

            case TypeCategory::Integer: {
                int reg2 = get_register_operand();
                int reg1 = get_register_operand();
                int dest_reg = allocate_register();
                _writer->write(Bytecode::AddI32, dest_reg, reg1, reg2);
                _register_stack.push(dest_reg);
                break;
            }

            /* case TypeCategory::Str: */
            /*     emit(Bytecode::StrAdd); */
            /*     break; */

            default:
                break;
                /* kore_unreachable(); */
        }
    }

    void BytecodeGenerator::visit(BoolExpression* expr) {
        auto reg = allocate_register();
        _writer->write(
            Bytecode::LoadBool,
            reg, expr->value() == "true" ? 1 : 0
        );
        _register_stack.push(reg);
    }

    void BytecodeGenerator::visit(IntegerExpression* expr) {
        auto reg = allocate_register();
        _writer->write(Bytecode::LoadI32, reg, expr->value());
        _register_stack.push(reg);
    }

    void BytecodeGenerator::visit(VariableAssignment* statement) {
        UNUSED_PARAM(statement);

        // a = 1 + 2

        int reg = get_register_operand();

        // NOTE: Use store global instruction for now until functions are
        // implemented
        _writer->write(Bytecode::StoreI32Global, reg);
    }

    int BytecodeGenerator::allocate_register() {
        return _registers++;
    }

    int BytecodeGenerator::get_register_operand() {
        auto reg = _register_stack.top();
        _register_stack.pop();

        return reg;
    }
}
