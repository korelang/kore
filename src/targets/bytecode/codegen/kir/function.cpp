#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"
#include "targets/bytecode/codegen/kir/function.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    namespace kir {
        Function::Function(FuncIndex index) : _index(index) {}

        Function::Function(FuncIndex index, const kore::Function& func)
            : _index(index), _func(&func) {}

        Function::~Function() {}

        Graph& Function::graph() {
            return _graph;
        }

        const Graph& Function::graph() const {
            return _graph;
        }

        void Function::add_instruction(Instruction instruction) {
            _graph.current_block().instructions.push_back(instruction);
        }

        void Function::set_register_state(Reg reg, RegisterState state) {
            _register_states[_graph.current_block().id][reg] = state;
        }

        Reg Function::allocate_register() {
            Reg reg = _reg_count++;
            set_register_state(reg, RegisterState::Available);

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
            // Destroy registers the in reverse order of how they were
            // introduced
            for (int reg = _reg_count - 1; reg >= 0; --reg) {
                switch (register_state(reg)) {
                    case RegisterState::Moved:
                        break;

                    case RegisterState::Available: {
                        if (register_type(reg)->is_value_type()) {
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

        void Function::load_constant(BoolExpression& expr) {
            add_instruction(
                Instruction(
                    InstructionType::LoadBool,
                    allocate_register(),
                    expr
                )
            );
        }

        void Function::load_constant(IntegerExpression& expr) {
            add_instruction(
                Instruction(
                    InstructionType::LoadInteger,
                    allocate_register(),
                    expr
                )
            );
        }

        void Function::load_constant(FloatExpression& expr) {
            add_instruction(
                Instruction(
                    InstructionType::LoadFloat,
                    allocate_register(),
                    expr
                )
            );
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
                    src,
                    dst,
                    assign
                )
            );
        }

        void Function::binop(BinaryExpression& expr, Reg left, Reg right) {
            add_instruction(
                Instruction(
                    InstructionType::Binop,
                    left,
                    right,
                    expr
                )
            );
        }

        void Function::branch(Reg condition, BlockId true_block, BlockId false_block) {
            add_instruction(
                Instruction(
                    InstructionType::Branch,
                    condition,
                    true_block,
                    false_block
                )
            );
        }

        void Function::allocate_array(ArrayExpression& expr, const std::vector<Reg> element_regs) {
            add_instruction(
                Instruction(
                    InstructionType::AllocateArray,
                    allocate_register(),
                    element_regs,
                    expr
                )
            );
        }

        void Function::destroy(Reg reg) {
            add_instruction(Instruction(Bytecode::Destroy, reg));
        }

        void Function::refinc(Reg reg) {
            add_instruction(Instruction(Bytecode::RefInc, reg));
        }

        void Function::_return() {
            add_instruction(Instruction(Bytecode::Ret));
        }

        void Function::_return(Reg retreg) {
            add_instruction(Instruction(Bytecode::Ret, retreg));
        }

        void Function::refdec(Reg reg) {
            add_instruction(Instruction(Bytecode::RefDec, reg));
        }

        FuncIndex Function::index() const noexcept {
            return _index;
        }

        std::string Function::name() const {
            return _func->name();
        }

        SourceLocation Function::location() const {
            return _func->location();
        }

        int Function::max_regs_used() const noexcept {
            return _max_regs_used;
        }

        int Function::code_size() const {
            // TODO
            return 0;
        }
    }
}
