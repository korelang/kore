#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "targets/bytecode/vm/config.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    namespace kir {
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

            destroy(reg);
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
                            destroy(reg);
                        }
                        break;
                    }

                    case RegisterState::MaybeMoved: {
                        break;
                    }
                }
            }
        }

        Reg Function::load_constant(BoolExpression& expr) {
            Reg reg = allocate_register();
            add_instruction(Instruction(InstructionType::LoadBool, reg, expr));

            return reg;
        }

        Reg Function::load_constant(IntegerExpression& expr, int index) {
            return load_constant(InstructionType::LoadInteger, expr, index);
        }

        Reg Function::load_constant(FloatExpression& expr, int index) {
            return load_constant(InstructionType::LoadFloat, expr, index);
        }

        Reg Function::load_global(Identifier& expr, Reg gidx) {
            auto reg = allocate_register();

            add_instruction(
                Instruction(
                    InstructionType::LoadGlobal,
                    reg,
                    gidx,
                    expr
                )
            );

            return reg;
        }

        void Function::move(VariableAssignment& assign, Reg dst, Reg src) {
            add_instruction(
                Instruction(
                    InstructionType::Move,
                    dst,
                    src,
                    assign
                )
            );
        }

        Reg Function::binop(BinaryExpression& expr, Reg left, Reg right) {
            auto reg = allocate_register();

            add_instruction(
                Instruction(
                    InstructionType::Binop,
                    reg,
                    left,
                    right,
                    expr
                )
            );

            return reg;
        }

        void Function::unconditional_jump(BlockId target_block_id) {
            add_instruction(Instruction(InstructionType::Value, Bytecode::Jump, target_block_id));
        }

        void Function::conditional_jump(Bytecode opcode, Reg condition, BlockId target_block_id) {
            add_instruction(Instruction(InstructionType::Value, opcode, condition, target_block_id));
        }

        Reg Function::allocate_array(ArrayExpression& expr, const std::vector<Reg> element_regs) {
            auto reg = allocate_register();

            add_instruction(
                Instruction(
                    InstructionType::AllocateArray,
                    reg,
                    element_regs,
                    expr
                )
            );

            return reg;
        }

        void Function::destroy(Reg reg) {
            add_instruction(Instruction(Bytecode::Destroy, reg));
        }

        void Function::refinc(Reg reg) {
            add_instruction(Instruction(Bytecode::RefInc, reg));
        }

        void Function::refdec(Reg reg) {
            add_instruction(Instruction(Bytecode::RefDec, reg));
        }

        void Function::call(
            kore::Bytecode opcode,
            Expression& expr,
            const std::vector<kore::Reg>& return_registers,
            const std::vector<kore::Reg>& arg_registers
        ) {
            add_instruction(Instruction::call(opcode, expr, arg_registers, return_registers));
        }

        void Function::_return() {
            add_instruction(Instruction(Bytecode::Ret));
        }

        void Function::_return(Reg retreg) {
            /* add_instruction(Instruction::ret(return_registers)); */

            // TODO: Support multiple return values
            std::vector<Reg> registers{ 1, retreg };

            add_instruction(Instruction(Bytecode::Ret, registers));
        }

        FuncIndex Function::index() const noexcept {
            return _index;
        }

        std::string Function::name() const {
            // If the function is not backed by a syntactic function then it is
            // a compiled-generated "main" function
            if (!_func) {
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

        Reg Function::load_constant(
            InstructionType inst_type,
            Expression& expr,
            int index
        ) {
            Reg reg = allocate_register();

            add_instruction(Instruction(inst_type, reg, expr, index));
            set_register_type(reg, expr.type());

            return reg;
        }

    }
}
