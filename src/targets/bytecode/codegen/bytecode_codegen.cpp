#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/bytecode_codegen.hpp"
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
    #define KORE_DEBUG_BYTECODE_GENERATOR_LOG(prefix, msg)
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

        // Write an implicit return at the end of compilation to pop
        // the main object's call frame
        _writer.write_1address(Bytecode::Ret, 0, current_object());

        return std::move(_objects.front());
    }

    Module::pointer&& BytecodeGenerator::compile(const Ast& ast) {
        reset();
        _module = std::make_unique<Module>();
        new_function_from_name(_MAIN_FUNC_NAME);

        for (auto const& statement : ast) {
            statement->accept_visit_only(*this);
        }

        // Write an implicit return at the end of compilation to pop
        // the main object's call frame
        _writer.write_1address(Bytecode::Ret, 0, current_object());

        // Store number of globals to allocate in this module
        _module->set_global_indices_count(_global_indices.size());

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

        Reg reg2 = get_register_operand();
        Reg reg1 = get_register_operand();

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
        auto reg = get_destination_register_or_allocate();

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
        auto reg = get_destination_register_or_allocate();
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
        auto reg = get_destination_register_or_allocate();
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

    void BytecodeGenerator::visit(StringExpression& expr) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("str", std::string());
        UNUSED_PARAM(expr);

        push_register(current_object()->allocate_register());
    }

    void BytecodeGenerator::visit(Identifier& identifier) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("identifier", identifier.name());

        auto entry = _scope_stack.find(identifier.name());
        auto obj = current_object();

        if (entry->is_global_scope()) {
            auto reg = obj->allocate_register();

            // TODO: Global values have a 16-bit address space which isn't used here
            _writer.write_2address(Bytecode::Gload, reg, entry->reg, obj);
            push_register(reg);
        } else {
            // There is a destination register available so move the
            // identifier's register into the destination
            if (_destination_register_stack.size() > 0) {
                Reg dst_reg = get_destination_register();
                _writer.write_2address(Bytecode::Move, dst_reg, entry->reg, obj);
            } else {
                // Otherwise, we push the identifier's register for further
                // compilation
                push_register(entry->reg);
            }
        }
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

        // Push destination register for the right-hand side expression
        push_destination_register(dest_reg);

        assignment.expression()->accept_visit_only(*this);

        if (_scope_stack.is_global_scope()) {
            add_global_index(dest_reg);
            _writer.write_2address(Bytecode::Gstore, dest_reg, get_register_operand(), obj);
        }
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
                condition->accept_visit_only(*this);
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
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("call", call.name())

        for (int i = 0; i < call.arg_count(); ++i) {
            call.arg(i)->accept_visit_only(*this);
        }

        auto obj = current_object();
        Reg func_index_reg = obj->allocate_register();
        Reg retreg = obj->allocate_register();
        auto called = _module->get_function(call.name());

        // Load the called function's index into a fresh register
        i32 index =_module->add_i32_constant(called->func_index());
        _writer.write_load(Bytecode::CloadI32, func_index_reg, index, obj);

        // Call instruction
        std::vector<std::uint8_t> bytes{
            Bytecode::Call,
            static_cast<std::uint8_t>(func_index_reg),
            static_cast<std::uint8_t>(call.arg_count()),
            // TODO: Use return count of called function here
            1 // So far only one return register
        };

        if (call.arg_count() > 0) {
            // Write argument instructions
            bytes.insert(
                bytes.end(),
                get_register_operands(call.arg_count()),
                _register_stack.cend()
            );
        }

        /* // Write return registers */
        bytes.push_back(retreg);

        _writer.write_bytes(bytes, obj);
        push_register(retreg);
    }

    void BytecodeGenerator::visit(Return& ret) {
        KORE_DEBUG_BYTECODE_GENERATOR_LOG("return", std::string())

        auto obj = current_object();

        // If the return statement returns an expression, generate code
        // for it, then get its register
        if (ret.expr()) {
            ret.expr()->accept_visit_only(*this);

            _writer.write_2address(
                Bytecode::Ret,
                1,
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
        _scope_stack.enter_function_scope(&func);

        // OR: func.add_parameters_to_scope(_scope_stack);
        for (int i = 0; i < func.arity(); ++i) {
            auto parameter = func.parameter(i);
            _scope_stack.insert(parameter, obj->allocate_register());
        }

        // Compile the body of the function
        for (auto& statement : func) {
            statement->accept_visit_only(*this);
        }

        auto last_statement = func.last_statement();

        // Add an implicit return for void functions without an explicit return
        if (last_statement && last_statement->statement_type() != StatementType::Return) {
            _writer.write_opcode(Bytecode::Ret, obj);
        }

        _scope_stack.leave_function_scope();
        end_function_compile();
    }

    void BytecodeGenerator::reset() {
        _register_stack.clear();
        _functions.clear();
        _objects.clear();
        _scope_stack.clear();
        _global_indices.clear();

        _current_object = nullptr;
        _module.reset(nullptr);
    }

    void BytecodeGenerator::push_register(Reg reg) {
        _register_stack.push_back(reg);
    }

    void BytecodeGenerator::push_destination_register(Reg reg) {
        _destination_register_stack.push_back(reg);
    }

    Reg BytecodeGenerator::get_register_operand() {
        auto reg = _register_stack.back();
        _register_stack.pop_back();

        return reg;
    }

    Reg BytecodeGenerator::get_destination_register() {
        auto reg = _destination_register_stack.back();
        _destination_register_stack.pop_back();

        return reg;
    }

    Reg BytecodeGenerator::get_destination_register_or_allocate() {
        if (_destination_register_stack.size() > 0) {
            return get_destination_register();
        }

        return current_object()->allocate_register();
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

    void BytecodeGenerator::add_global_index(Reg reg) {
        _global_indices.insert(reg);
    }

    void BytecodeGenerator::new_function_from_name(const std::string& name) {
        _current_object = _module->new_function_from_name(name);
        _functions.push_back(name);
    }
}

#undef KORE_DEBUG_BYTECODE_GENERATOR_LOG
