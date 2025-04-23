#ifndef KORE_KIR_FUNCTION_HPP
#define KORE_KIR_FUNCTION_HPP

#include "ast/expressions/expressions.hpp"
#include "ast/statements/function.hpp"
#include "targets/bytecode/codegen/kir/graph.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"
#include "targets/bytecode/codegen/kir/register_state.hpp"
#include "targets/bytecode/register.hpp"

namespace kore {
    class VariableAssignment;

    namespace kir {
        using FuncIndex = unsigned int;

        /// A function represented as KIR
        class Function final {
            public:
                Function(FuncIndex index);
                Function(FuncIndex index, const kore::Function* func);
                virtual ~Function();

                Graph& graph();
                const Graph& graph() const;
                void add_instruction(Instruction instruction);
                void set_register_state(Reg reg, RegisterState state);
                void set_register_type(Reg reg, const Type* type);
                Reg allocate_register();
                std::vector<Reg> allocate_registers(int count);
                void free_register(Reg reg);
                void free_registers();
                RegisterState register_state(Reg reg);
                const Type* register_type(Reg reg);

                void emit_reg3(Bytecode opcode, Reg reg1, Reg reg2, Reg reg3);

                Reg emit_load(Bytecode opcode, int index);
                Reg emit_load(Bytecode opcode, Expression& expr, int index);
                Reg emit_load_function(int func_index, Bytecode opcode);
                Reg emit_binop(BinaryExpression& expr, Reg left, Reg right);
                void emit_unconditional_jump(BlockId target_block_id);
                void emit_move(Reg src, Reg dst);
                void emit_conditional_jump(Bytecode opcode, Reg condition, BlockId target_block_id);
                Reg emit_allocate_array(int size);
                void emit_destroy(Reg reg);
                /* void destroy(Expression& expr, Reg reg); */
                void emit_refinc(Reg reg);
                void emit_refdec(Reg reg);
                void emit_call(
                    Reg func_reg,
                    const std::vector<kore::Reg>& arg_registers,
                    const std::vector<kore::Reg>& return_registers
                );
                void emit_return();
                void emit_return(const std::vector<Reg>& regs);

                FuncIndex index() const noexcept;
                std::string name() const;
                SourceLocation location() const;
                int max_regs_used() const noexcept;
                int code_size() const;

            private:
                FuncIndex _index;
                const kore::Function* _func;
                Graph _graph;
                int _code_size;

                // For now, we just use a very simple per-function register
                // allocator with a maximum of 256 registers that just bumps a
                // register count
                int _reg_count = 0;
                int _max_regs_used = 0;

                // The state of a register in a basic block
                std::unordered_map<BlockId, std::unordered_map<Reg, RegisterState>> _register_states;

                // The type contained in a given register, this is important
                // when checking if we need to or can free what is stored in it
                std::unordered_map<Reg, const Type*> _register_types;
        };
    }
}

#endif // KORE_KIR_FUNCTION_HPP
