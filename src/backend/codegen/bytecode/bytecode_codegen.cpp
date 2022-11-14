#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/bytecode/bytecode_codegen.hpp"
#include "utils/unused_parameter.hpp"

#if defined(KORE_DEBUG_BYTECODE_GENERATOR) || defined(KORE_DEBUG)
    #include <iostream>

    #define KORE_DEBUG_BYTECODE_GENERATOR_LOG(prefix, msg) {\
        std::cerr << "[compiler] " << prefix;\
        \
        if (msg.length() > 0) {\
            std::cerr << " (" << msg << ")";\
        }\
        \
        std::cerr << std::endl;\
    }
#else
    #define KORE_DEBUG_BYTECODE_GENERATOR_LOG(prefix)
#endif

namespace kore {
    const std::string BytecodeGenerator::_bytecode_version = "1.0.0";
    const std::string BytecodeGenerator::_MAIN_FUNC_NAME = "<main>";

    std::map<TypeCategory, std::map<BinOp, Bytecode>> _binop_map = {
        {
            TypeCategory::Integer32, {
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
            TypeCategory::Integer64, {
                {BinOp::Plus,     Bytecode::AddI64},
                {BinOp::Minus,    Bytecode::SubI64},
                {BinOp::Mult,     Bytecode::MultI64},
                {BinOp::Pow,      Bytecode::PowI64},
                {BinOp::Div,      Bytecode::DivI64},
                {BinOp::Lt,       Bytecode::LtI64},
                {BinOp::Gt,       Bytecode::GtI64},
                {BinOp::Le,       Bytecode::LeI64},
                {BinOp::Ge,       Bytecode::GeI64},
                {BinOp::Equal,    Bytecode::EqI64},
                {BinOp::NotEqual, Bytecode::NeqI64},
            },
        },
        {
            TypeCategory::Float32, {
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
        {
            TypeCategory::Float64, {
                {BinOp::Plus,     Bytecode::AddF64},
                {BinOp::Minus,    Bytecode::SubF64},
                {BinOp::Mult,     Bytecode::MultF64},
                {BinOp::Pow,      Bytecode::PowF64},
                {BinOp::Div,      Bytecode::DivF64},
                {BinOp::Lt,       Bytecode::LtF64},
                {BinOp::Gt,       Bytecode::GtF64},
                {BinOp::Le,       Bytecode::LeF64},
                {BinOp::Ge,       Bytecode::GeF64},
                {BinOp::Equal,    Bytecode::EqF64},
                {BinOp::NotEqual, Bytecode::NeqF64},
            },
        },
    };

    BytecodeGenerator::BytecodeGenerator() {}

    BytecodeGenerator::~BytecodeGenerator() {}

    CompiledObject::pointer&& BytecodeGenerator::compile(Statement& statement) {
        reset();
        statement.accept(*this);

        return std::move(_objects.front());
    }

    Module::pointer&& BytecodeGenerator::compile(const Ast& ast) {
        reset();
        _module = std::make_unique<Module>();
        new_function_from_name(_MAIN_FUNC_NAME);

        for (auto const& statement : ast) {
            statement->accept_visit_only(*this);
        }

        return std::move(_module);
    }

    Bytecode BytecodeGenerator::get_binop_instruction(
        TypeCategory type_category,
        BinOp binop
    ) {
        return _binop_map[type_category][binop];
    }

    void BytecodeGenerator::visit(BinaryExpression& expr) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("binop", expr.op_string())

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

        push_register(dest_reg);
    }

    void BytecodeGenerator::visit(BoolExpression& expr) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("bool", std::string())

        auto obj = current_object();
        auto reg = obj->allocate_register();

        _writer.write_load(
            Bytecode::LoadBool,
            reg, expr.value() == "true" ? 1 : 0,
            obj
        );
        push_register(reg);
    }

    void BytecodeGenerator::visit(IntegerExpression& expr) {
        int num_bits = expr.type()->category() == TypeCategory::Integer32 ? 32 : 64;
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("i" + std::to_string(num_bits), std::string())

        auto obj = current_object();
        auto reg = obj->allocate_register();
        int index;
        Bytecode opcode;

        if (num_bits == 32) {
            index =_module->add_i32_constant(expr.value());
            opcode = Bytecode::CloadI32;
        } else {
            index =_module->add_i64_constant(expr.value());
            opcode = Bytecode::CloadI64;
        }

        _writer.write_load(opcode, reg, index, obj);
        push_register(reg);
    }

    void BytecodeGenerator::visit(FloatExpression& expr) {
        int num_bits = expr.type()->category() == TypeCategory::Float32 ? 32 : 64;
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("f" + std::to_string(num_bits), std::string())

        auto obj = current_object();
        auto reg = obj->allocate_register();
        int index;
        Bytecode opcode;

        if (num_bits == 32) {
            index = _module->add_f32_constant(expr.value());
            opcode = Bytecode::CloadF32;
        } else {
            index = _module->add_f64_constant(expr.value());
            opcode = Bytecode::CloadF64;
        }

        _writer.write_load(opcode, reg, index, obj);
        push_register(reg);
    }

    void BytecodeGenerator::visit(Identifier& identifier) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("identifier", identifier.name())

        auto entry = _scope_stack.find(identifier.name());
        push_register(entry->reg);
    }

    void BytecodeGenerator::visit(VariableAssignment& assignment) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("assignment", assignment.identifier()->name())

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
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("if", std::string())

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

    // TODO: What functions are we calling?
    // TODO: How do we specify the return register(s)?
    void BytecodeGenerator::visit(class Call& call) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("call", call.name())

        // Generate code in reverse order of arguments so we can
        // get the registers in the correct order for the call
        /* for (int i = call.arg_count() - 1; i >= 0; --i) { */
        for (int i = 0; i < call.arg_count(); ++i) {
            call.arg(i)->accept_visit_only(*this);
        }

        auto obj = current_object();
        std::uint8_t func_index = 0;
        Reg retreg = obj->allocate_register();

        // TODO: We also need to specify what function to call here (probably
        // via a register)
        std::vector<std::uint8_t> bytes{
            Bytecode::Call,
            func_index,
            1, // So far only one return register
            static_cast<std::uint8_t>(call.arg_count())
        };

        bytes.push_back(retreg);

        bytes.insert(
            bytes.end(),
            get_register_operands(call.arg_count()),
            _register_stack.cend()
        );

        _writer.write_bytes(bytes, obj);
        push_register(retreg);
    }

    void BytecodeGenerator::visit(Return& ret) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("return", std::string())
        UNUSED_PARAM(ret);

        auto obj = current_object();

        // If the return statement returns an expression, generate code
        // for it, then get its register
        if (ret.expr()) {
            ret.expr()->accept_visit_only(*this);
            _writer.write_1address(
                Bytecode::Ret,
                get_register_operand(),
                obj
            );
        } else {
            // Otherwise return zero registers
            _writer.write_1address(Bytecode::Ret, 0, obj);
        }
    }

    void BytecodeGenerator::visit(Function& func) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("function", func.name())

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

    void BytecodeGenerator::reset() {
        _register_stack.clear();
        _functions.clear();
        _objects.clear();
        _scope_stack.clear();

        _current_object = nullptr;
        _module.reset(nullptr);
    }

    void BytecodeGenerator::push_register(Reg reg) {
        _register_stack.push_back(reg);
    }

    int BytecodeGenerator::get_register_operand() {
        auto reg = _register_stack.back();
        _register_stack.pop_back();

        return reg;
    }

    BytecodeGenerator::reg_iterator BytecodeGenerator::get_register_operands(int count) {
        return _register_stack.cbegin() + (_register_stack.size() - count);
    }

    CompiledObject* BytecodeGenerator::current_object() {
        return _current_object;
    }

    void BytecodeGenerator::start_function_compile(const Function& func) {
        _current_object = _module->new_function(func);
        _functions.push_back(func.name());
    }

    void BytecodeGenerator::end_function_compile() {
        _functions.pop_back();

        if (_functions.size() > 0) {
            _current_object = _module->get_function(_functions.back());
        }
    }

    void BytecodeGenerator::new_function_from_name(const std::string& name) {
        _current_object = _module->new_function_from_name(name);
        _functions.push_back(name);
    }
}

#undef KORE_DEBUG_BYTECODE_GENERATOR_LOG
