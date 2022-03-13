#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "backend/vm/vm.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    const std::string BytecodeGenerator::_bytecode_version = "1.0.0";

    std::map<TypeCategory, std::map<BinOp, Bytecode>> _binop_map = {
        {
            TypeCategory::Integer, {
                {BinOp::Plus,  Bytecode::AddI32},
                {BinOp::Minus, Bytecode::SubI32},
                {BinOp::Mult,  Bytecode::MultI32},
                {BinOp::Pow,   Bytecode::PowI32},
                {BinOp::Div,   Bytecode::DivI32},
                {BinOp::Lt,    Bytecode::LtI32},
            },
        },
        {
            TypeCategory::Float, {
                {BinOp::Plus,  Bytecode::AddF32},
                {BinOp::Minus, Bytecode::SubF32},
                {BinOp::Mult,  Bytecode::MultF32},
                {BinOp::Pow,   Bytecode::PowF32},
                {BinOp::Div,   Bytecode::DivF32},
                {BinOp::Lt,    Bytecode::LtF32},
            },
        },
    };

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

    Bytecode BytecodeGenerator::get_binop_instruction(
        TypeCategory type_category,
        BinOp binop
    ) {
        return _binop_map[type_category][binop];
    }

    void BytecodeGenerator::visit(BinaryExpression* expr) {
        int reg2 = get_register_operand();
        int reg1 = get_register_operand();
        int dest_reg = allocate_register();

        _writer->write(
            get_binop_instruction(expr->type()->category(), expr->op()),
            dest_reg,
            reg1,
            reg2
        );

        _register_stack.push(dest_reg);
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

    void BytecodeGenerator::visit(FloatExpression* expr) {
        auto reg = allocate_register();
        _writer->write(Bytecode::LoadF32, reg, expr->value());
        _register_stack.push(reg);
    }

    void BytecodeGenerator::visit(VariableAssignment* statement) {
        UNUSED_PARAM(statement);

        int reg = get_register_operand();

        // NOTE: Use store global instruction for now until functions are
        // implemented
        _writer->write(Bytecode::StoreI32Global, reg);
    }

    int BytecodeGenerator::allocate_register() {
        if (_registers >= KORE_VM_MAX_REGISTERS) {
            throw std::runtime_error("No more registers");
        }

        return _registers++;
    }

    int BytecodeGenerator::get_register_operand() {
        auto reg = _register_stack.top();
        _register_stack.pop();

        return reg;
    }
}
