#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
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
        _objects.clear();
        new_compiled_object("<main>");

        for (auto const& statement : ast) {
            statement->accept_visit_only(*this);
        }
    }

    Bytecode BytecodeGenerator::get_binop_instruction(
        TypeCategory type_category,
        BinOp binop
    ) {
        return _binop_map[type_category][binop];
    }

    void BytecodeGenerator::visit(BinaryExpression& expr) {
        auto obj = current_object();
        int dest_reg = obj->allocate_register();

        expr.left()->accept_visit_only(*this);
        expr.right()->accept_visit_only(*this);

        int reg2 = get_register_operand();
        int reg1 = get_register_operand();

        _writer.write_3address(
            get_binop_instruction(expr.type()->category(), expr.op()),
            dest_reg,
            reg1,
            reg2,
            obj
        );

        obj->free_registers(2);

        _register_stack.push_back(dest_reg);
    }

    void BytecodeGenerator::visit(BoolExpression& expr) {
        auto obj = current_object();
        auto reg = obj->allocate_register();

        _writer.write_load(
            Bytecode::LoadBool,
            reg, expr.value() == "true" ? 1 : 0,
            obj
        );
        _register_stack.push_back(reg);
    }

    void BytecodeGenerator::visit(IntegerExpression& expr) {
        auto obj = current_object();
        auto reg = obj->allocate_register();

        _writer.write_load(Bytecode::CloadI32, reg, expr.value(), obj);
        _register_stack.push_back(reg);
    }

    void BytecodeGenerator::visit(FloatExpression& expr) {
        auto obj = current_object();
        auto reg = obj->allocate_register();

        _writer.write_load(Bytecode::CloadF32, reg, expr.value(), obj);
        _register_stack.push_back(reg);
    }

    void BytecodeGenerator::visit(Identifier& identifier) {
        auto entry = _scope_stack.find(identifier.name());
        _register_stack.push_back(entry->reg);
    }

    void BytecodeGenerator::visit(VariableAssignment& assignment) {
        auto obj = current_object();
        auto entry = _scope_stack.find_inner(assignment.identifier()->name());
        Reg dest_reg;

        // Allocate a new destination register or use the one in the
        // current scope
        if (!entry) {
            dest_reg = obj->allocate_register();
            _scope_stack.insert(assignment.identifier(), dest_reg);
        } else {
            dest_reg = entry->reg;
        }

        assignment.expression()->accept_visit_only(*this);
        Reg reg = get_register_operand();

        if (dest_reg != reg) {
            // A little pre-optimisation: Do not emit moves from and to the
            // same register
            _writer.write_2address(Bytecode::Move, dest_reg, reg, obj);
        }

        // Free the register used for the right-hand side expression
        obj->free_registers(1);
    }

    void BytecodeGenerator::visit(IfStatement& ifstatement) {
        auto obj = current_object();

        // Labels for all unconditional jumps to the end of the conditional
        // that need to be backpatched after all code has been generated
        std::vector<Label> labels;

        for (auto& branch : ifstatement) {
            auto condition = branch->condition();
            Label label;

            if (condition) {
                branch->condition()->accept_visit_only(*this);
                Reg reg = get_register_operand();
                label = _writer.write_jump(Bytecode::JumpIfNot, reg, obj);
            }

            _scope_stack.enter();
            for (auto& statement : *branch) {
                statement->accept_visit_only(*this);
            }
            _scope_stack.leave();

            if (condition) {
                // Write an unconditional jump to skip the entire conditional
                // statement after the branch's body has been executed
                labels.push_back(_writer.write_jump(Bytecode::Jump, obj));

                _writer.patch_jump(label, obj);
            }
        }

        for (Label label : labels) {
            // Patch all jumps to end of the conditional statement
            _writer.patch_jump(label, obj);
        }
    }

    void BytecodeGenerator::visit(class Call& call) {
        // Generate code in reverse order of arguments so we can
        // get the registers in the correct order for the call
        for (int i = call.arg_count() - 1; i >= 0; --i) {
            call.arg(i)->accept_visit_only(*this);
        }

        auto obj = current_object();
        auto first = _register_stack.cbegin() + (_register_stack.size() - call.arg_count());

        _writer.write_variable_length(
            Bytecode::Call,
            call.arg_count(),
            first,
            _register_stack.cend(),
            obj
        );
    }

    void BytecodeGenerator::visit(Return& ret) {
        UNUSED_PARAM(ret);

        auto obj = current_object();

        // If the return statement returns an expression, get its register
        // and return it, otherwise just return
        if (ret.expr()) {
            auto reg = get_register_operand();
            _writer.write_1address(Bytecode::RetReg, reg, obj);
        } else {
            _writer.write_opcode(Bytecode::Ret, obj);
        }
    }

    void BytecodeGenerator::visit(Function& func) {
        start_function_compile(func);
        auto obj = current_object();

        // Enter a new function scope and add all function
        // arguments to that scope
        _scope_stack.enter_function_scope();

        // OR: func.add_parameters_to_scope(_scope_stack);
        for (int i = 0; i < func.arity(); ++i) {
            auto parameter = func.parameter(i);
            _scope_stack.insert(parameter, obj->allocate_register());
        }

        // Compile the body of the function
        for (auto& statement : func) {
            statement->accept_visit_only(*this);
        }

        _scope_stack.leave();
        end_function_compile();
    }

    int BytecodeGenerator::get_register_operand() {
        auto reg = _register_stack.back();
        _register_stack.pop_back();

        return reg;
    }

    BytecodeGenerator::RegIterator BytecodeGenerator::get_register_operands(int count) {
        return _register_stack.begin() + (_register_stack.size() - count);
    }

    CompiledObject* BytecodeGenerator::current_object() {
        return _current_object;
    }

    void BytecodeGenerator::start_function_compile(const Function& func) {
        new_compiled_object(&func);
        _current_object = _objects.back().get();
    }

    void BytecodeGenerator::end_function_compile() {
        _current_object = _objects.front().get();
    }
}
