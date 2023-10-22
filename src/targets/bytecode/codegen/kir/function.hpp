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
                void free_register(Reg reg);
                void free_registers();
                RegisterState register_state(Reg reg);
                const Type* register_type(Reg reg);

                Reg load_constant(BoolExpression& expr);
                /* void load_constant(CharExpression& expr); */
                Reg load_constant(IntegerExpression& expr, int index);
                Reg load_constant(FloatExpression& expr, int index);
                Reg load_global(Identifier& expr, Reg gidx);
                Reg binop(BinaryExpression& expr, Reg left, Reg right);
                void move(VariableAssignment& assign, Reg src, Reg dst);
                void branch(Reg condition, BlockId true_block, BlockId false_block);
                Reg allocate_array(ArrayExpression& expr, const std::vector<Reg> element_regs);
                void destroy(Reg reg);
                /* void destroy(Expression& expr, Reg reg); */
                void refinc(Reg reg);
                void refdec(Reg reg);
                void call(
                    kore::Bytecode opcode,
                    Expression& expr, // TODO: This doesn't need to be mutable
                    const std::vector<kore::Reg>& return_registers,
                    const std::vector<kore::Reg>& arg_registers
                );
                void _return();
                void _return(Reg retreg);

                FuncIndex index() const noexcept;
                std::string name() const;
                SourceLocation location() const;
                int max_regs_used() const noexcept;
                int code_size() const;

            private:
                Reg load_constant(InstructionType inst_type, Expression& expr, int index);

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
