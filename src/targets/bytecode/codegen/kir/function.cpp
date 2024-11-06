#include <numeric>

#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "targets/bytecode/vm/config.hpp"
#include "types/type_category.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    namespace kir {
        std::map<TypeCategory, std::map<BinOp, Bytecode>> _binary_operator_opcodes = {
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

        Function::Function(FuncIndex index)
            : _index(index),
              _func(nullptr),
              _code_size(0) {}

        Function::Function(FuncIndex index, const kore::Function* func)
            : _index(index), _func(func), _code_size(0) {}

        Function::~Function() {}

        Graph& Function::graph() {
            return _graph;
        }

        const Graph& Function::graph() const {
            return _graph;
        }

        void Function::add_instruction(Instruction instruction) {
            _graph.current_block().instructions.push_back(instruction);
            ++_code_size;
        }

        void Function::set_register_state(Reg reg, RegisterState state) {
            _register_states[_graph.current_block().id][reg] = state;
        }

        void Function::set_register_type(Reg reg, const Type* type) {
            _register_types[reg] = type;
        }

        Reg Function::allocate_register() {
            if (_reg_count >= vm::KORE_VM_MAX_REGISTERS) {
                throw std::runtime_error("register overflow");
            }

            Reg reg = _reg_count++;
            set_register_state(reg, RegisterState::Available);
            _max_regs_used = std::max(_max_regs_used, _reg_count);

            return reg;
        }

        std::vector<Reg> Function::allocate_registers(int count) {
            if (_reg_count + count >= vm::KORE_VM_MAX_REGISTERS) {
                throw std::runtime_error("register overflow");
            }

            std::vector<Reg> regs{ count };

            for (int idx = 0; idx < count; ++idx) {
                Reg reg = _reg_count++;
                regs.push_back(reg);
                set_register_state(reg, RegisterState::Available);
            }

            _max_regs_used = std::max(_max_regs_used, _reg_count);

            return regs;
        }

        RegisterState Function::register_state(Reg reg) {
            auto bb = _graph.current_block();
            auto entry = _register_states[bb.id].find(reg);

            if (entry != _register_states[bb.id].end()) {
                // We already determined the state of the register in this block
                return entry->second;
            }

            // TODO
            return RegisterState::Available;
        }

        const Type* Function::register_type(Reg reg) {
            auto entry = _register_types.find(reg);

            if (entry == _register_types.end()) {
                return Type::unknown();
            }

            return entry->second;
        }

        void Function::free_register(Reg reg) {
            if (register_type(reg)->is_value_type()) {
                return;
            }

            emit_destroy(reg);
            set_register_state(reg, RegisterState::Moved);
        }

        void Function::free_registers() {
            // Destroy registers in the reverse order of how they were introduced
            for (int reg = _reg_count - 1; reg >= 0; --reg) {
                switch (register_state(reg)) {
                    case RegisterState::Moved:
                        break;

                    case RegisterState::Available: {
                        if (!register_type(reg)->is_value_type()) {
                            emit_destroy(reg);
                        }
                        break;
                    }

                    case RegisterState::MaybeMoved: {
                        break;
                    }
                }
            }
        }

        // TODO: Remember to set register types
        // TODO: Do we need functions for each expression/statement? Maybe just for
        // each type of instruction + the expression for setting register types?

        void Function::emit_reg3(Bytecode opcode, Reg reg1, Reg reg2, Reg reg3) {
            add_instruction(Instruction{
                opcode,
                ThreeRegisters{ reg1, reg2, reg3 }
            });
        }

        Reg Function::emit_load(Bytecode opcode, int index) {
            Reg reg = allocate_register();

            add_instruction(Instruction{ opcode, RegisterAndValue{ reg, index } });
            set_register_type(reg, Type::get_type_from_category(TypeCategory::Integer32));

            return reg;
        }

        Reg Function::emit_load(Bytecode opcode, Expression& expr, int index) {
            Reg reg = allocate_register();

            add_instruction(Instruction{ opcode, RegisterAndValue{ reg, index } });
            set_register_type(reg, expr.type());

            return reg;
        }

        Reg Function::emit_load_function(int func_index) {
            Reg reg = allocate_register();

            add_instruction(
                Instruction{ Bytecode::LoadFunction, RegisterAndValue{ reg, func_index } }
            );

            return reg;
        }

        void Function::emit_move(Reg dst, Reg src) {
            add_instruction(Instruction{ Bytecode::Move, TwoRegisters{ dst, src } });
        }

        Reg Function::emit_binop(BinaryExpression& expr, Reg left, Reg right) {
            auto reg = allocate_register();
            auto opcode = _binary_operator_opcodes[expr.type()->category()][expr.op()];

            add_instruction(Instruction{ opcode, ThreeRegisters{ reg, left, right } });

            return reg;
        }

        void Function::emit_unconditional_jump(BlockId target_block_id) {
            add_instruction(Instruction{ Bytecode::Jump, Value{ target_block_id } });
        }

        void Function::emit_conditional_jump(
            Bytecode opcode,
            Reg condition,
            BlockId target_block_id
        ) {
            add_instruction(
                Instruction{ opcode, RegisterAndValue{ condition, target_block_id } }
            );
        }

        Reg Function::emit_allocate_array(int size) {
            auto reg = allocate_register();

            add_instruction(
                Instruction{ Bytecode::ArrayAlloc, RegisterAndValue{ reg, size } }
            );

            return reg;
        }

        void Function::emit_destroy(Reg reg) {
            add_instruction(Instruction{ Bytecode::Destroy, OneRegister{ reg } });
        }

        void Function::emit_refinc(Reg reg) {
            add_instruction(Instruction{ Bytecode::RefInc, OneRegister{ reg } });
        }

        void Function::emit_refdec(Reg reg) {
            add_instruction(Instruction{ Bytecode::RefDec, OneRegister{ reg } });
        }

        void Function::emit_call(
            Reg func_reg,
            const std::vector<kore::Reg>& arg_registers,
            const std::vector<kore::Reg>& return_registers
        ) {
            add_instruction(
                Instruction{
                    Bytecode::Call,
                    CallV{ func_reg, arg_registers, return_registers },
                }
            );
        }

        void Function::emit_return() {
            add_instruction(Instruction{ Bytecode::Ret, ReturnV{} });
        }

        void Function::emit_return(const std::vector<Reg>& regs) {
            add_instruction(Instruction{ Bytecode::Ret, ReturnV{ regs } });
        }

        FuncIndex Function::index() const noexcept {
            return _index;
        }

        std::string Function::name() const {
            // If the function is not backed by an AST function then it is a
            // compiled-generated "main" function
            if (!_func) {
                // TODO: Make "<main>" a constant somewhere logic
                return "<main>";
            }

            return _func->name();
        }

        SourceLocation Function::location() const {
            // If the function is not backed by a syntactic function then it is
            // a compiled-generated "main" function
            if (!_func) {
                return SourceLocation::unknown;
            }

            return _func->location();
        }

        int Function::max_regs_used() const noexcept {
            return _max_regs_used;
        }

        int Function::code_size() const {
            return _code_size;
        }
    }
}
