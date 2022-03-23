#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "backend/vm/vm.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    const std::string BytecodeGenerator::_bytecode_version = "1.0.0";

    std::map<TypeCategory, std::map<BinOp, Bytecode>> _binop_map = {
        {
            TypeCategory::Integer, {
                {BinOp::Plus,     Bytecode::AddI32},
                {BinOp::Minus,    Bytecode::SubI32},
                {BinOp::Mult,     Bytecode::MultI32},
                {BinOp::Pow,      Bytecode::PowI32},
                {BinOp::Div,      Bytecode::DivI32},
                {BinOp::Lt,       Bytecode::LtI32},
                {BinOp::Gt,       Bytecode::GtI32},
                {BinOp::Le,       Bytecode::LeI32},
                {BinOp::Ge,       Bytecode::GeI32},
                {BinOp::Equal,    Bytecode::EqI32},
                {BinOp::NotEqual, Bytecode::NeqI32},
            },
        },
        {
            TypeCategory::Float, {
                {BinOp::Plus,     Bytecode::AddF32},
                {BinOp::Minus,    Bytecode::SubF32},
                {BinOp::Mult,     Bytecode::MultF32},
                {BinOp::Pow,      Bytecode::PowF32},
                {BinOp::Div,      Bytecode::DivF32},
                {BinOp::Lt,       Bytecode::LtF32},
                {BinOp::Gt,       Bytecode::GtF32},
                {BinOp::Le,       Bytecode::LeF32},
                {BinOp::Ge,       Bytecode::GeF32},
                {BinOp::Equal,    Bytecode::EqF32},
                {BinOp::NotEqual, Bytecode::NeqF32},
            },
        },
    };

    BytecodeGenerator::BytecodeGenerator(ScopeStack& scope_stack)
        : _scope_stack(scope_stack) {}

    BytecodeGenerator::~BytecodeGenerator() {}

    void BytecodeGenerator::compile(const Ast& ast) {
        for (auto const& statement : ast) {
            statement->accept(this);
        }
    }

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

        _writer.write_3address(
            get_binop_instruction(expr->type()->category(), expr->op()),
            dest_reg,
            reg1,
            reg2
        );

        _register_stack.push(dest_reg);
    }

    void BytecodeGenerator::visit(BoolExpression* expr) {
        auto reg = allocate_register();
        _writer.write_1address(
            Bytecode::LoadBool,
            reg, expr->value() == "true" ? 1 : 0
        );
        _register_stack.push(reg);
    }

    void BytecodeGenerator::visit(IntegerExpression* expr) {
        auto reg = allocate_register();
        _writer.write_1address(Bytecode::LoadI32, reg, expr->value());
        _register_stack.push(reg);
    }

    void BytecodeGenerator::visit(FloatExpression* expr) {
        auto reg = allocate_register();
        _writer.write_1address(Bytecode::LoadF32, reg, expr->value());
        _register_stack.push(reg);
    }

    void BytecodeGenerator::visit(Identifier* expr) {
        auto entry = _scope_stack.find(expr->name());
        _register_stack.push(entry->reg);
    }

    void BytecodeGenerator::visit(VariableAssignment* statement) {
        auto entry = _scope_stack.find_inner(statement->identifier()->name());
        Reg dest_reg;
        Reg reg = get_register_operand();

        if (!entry) {
            dest_reg = allocate_register();
            _scope_stack.insert(statement->identifier(), dest_reg);
        } else {
            dest_reg = entry->reg;
        }

        _writer.write_2address(Bytecode::Move, dest_reg, reg);
    }

    void BytecodeGenerator::visit(IfStatement* statement) {
        // Labels for all unconditional jumps to the end of the conditional
        // that need to be backpatched after all code has been generated
        std::vector<Label> labels;

        for (auto& branch : *statement) {
            auto condition = branch->condition();
            Label label;

            if (condition) {
                branch->condition()->accept(this);
                label = _writer.write_jump(Bytecode::JumpIfNot, get_register_operand());
            }

            _scope_stack.enter(false);
            for (auto& statement : *branch) {
                statement->accept(this);
            }
            _scope_stack.leave();

            if (condition) {
                // Write an unconditional jump to skip the entire conditional
                // statement after the branch's body has been executed
                labels.push_back(_writer.write_jump(Bytecode::Jump));

                _writer.patch_jump(label);
            }
        }

        for (Label label : labels) {
            // Patch all jumps to end of the conditional statement
            _writer.patch_jump(label);
        }
    }

    bool BytecodeGenerator::precondition(Branch* branch) {
        UNUSED_PARAM(branch);
        return true;
    }

    /* bool BytecodeGenerator::postcondition(Branch* branch) { */
    /*     UNUSED_PARAM(branch); */
    /*     _scope_stack.leave(); */
    /*     return false; */
    /* } */

    code_iterator BytecodeGenerator::begin() const {
        return _writer.begin();
    }

    code_iterator BytecodeGenerator::end() const {
        return _writer.end();
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
