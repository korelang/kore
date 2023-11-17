#ifndef KORE_KIR_INSTRUCTION_HPP
#define KORE_KIR_INSTRUCTION_HPP

#include "ast/expressions/expressions.hpp"
#include "ast/source_location.hpp"
#include "ast/statements/statement.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/block_id.hpp"
#include "targets/bytecode/register.hpp"
#include "types/type_category.hpp"

namespace kore {
    namespace kir {
        enum class InstructionType {
            AllocateArray = 0,
            Binop,
            BuiltinFunctionCall,
            Call,
            LoadBool,
            LoadInteger,
            LoadFloat,
            LoadGlobal,
            Move,
            Branch,
            Raw,
            Value,
        };

        // FIX: Convert this to use std::variant

        /// A KIR instruction. Whereas instructions emitted by code generation are
        /// actual (lower-level) executable instructions, KIR instructions are at
        /// a higher level and are not per-se executable but are used for optimisation
        /// and code generation.
        ///
        /// A good example is the branch instruction which represents a conditional jump
        /// to one of two basic blocks. When generating code, this is translated into
        /// an actual jump instruction that jumps with a byte offset rather than a basic
        /// block index.
        class Instruction {
            public:
                Instruction();
                Instruction(InstructionType type);
                // TODO: Specialise these constructors into static calls if they
                // are only used once
                Instruction(InstructionType type, Expression& expr);
                Instruction(InstructionType type, kore::Reg reg1, Expression& expr);
                Instruction(InstructionType type, Bytecode opcode, int value);
                Instruction(InstructionType type, Bytecode opcode, Reg reg, int value);
                Instruction(InstructionType type, kore::Reg reg1, Expression& expr, int value);
                Instruction(InstructionType type, kore::Reg reg, const std::vector<Reg>& registers, Expression& expr);
                Instruction(InstructionType type, kore::Reg reg1, kore::Reg reg2, Expression& expr);
                Instruction(InstructionType type, kore::Reg reg1, kore::Reg reg2, kore::Reg reg3, Expression& expr);
                Instruction(InstructionType type, kore::Reg reg1, kore::Reg reg2, Statement& statement);
                Instruction(InstructionType type, kore::Reg reg, BlockId bb1, BlockId bb2);
                Instruction(Bytecode opcode);
                Instruction(Bytecode opcode, kore::Reg reg);
                Instruction(Bytecode opcode, const std::vector<Reg>& registers);
                virtual ~Instruction();

                static Instruction with_offset(kore::Bytecode opcode, int pos, int reg1, int offset);
                static Instruction call(
                    kore::Bytecode opcode,
                    Expression& expr,
                    const std::vector<kore::Reg>& return_registers,
                    const std::vector<kore::Reg>& arg_registers
                );

                InstructionType type() const noexcept;
                /* short register_count() const; */
                kore::Reg reg1() const;
                kore::Reg reg2() const;
                kore::Reg reg3() const;
                kore::Reg& operator[](int index);

                BlockId bb1() const;
                BlockId bb2() const;

                template<typename T>
                const T* expr_as() const {
                    static_assert(
                        std::is_base_of_v<Expression, T> == true,
                        "T must be a derived class of Expression"
                    );

                    // TODO: Probably cast away const and readd it to avoid
                    // duplicate const
                    return static_cast<const T*>(_expr);
                }

                const Expression* expr() const;
                const Statement* statement() const;
                int value() const noexcept;
                Bytecode opcode() const;
                std::vector<kore::Reg> registers() const; // TODO: Make this return a const reference
                std::string registers_as_string() const;

            private:
                InstructionType _type;

                Bytecode _opcode;

                // We are a bit lazy here and clump up all types of instructions in one
                // class since I really didn't want to define a new class hierarchy...
                Expression* _expr;
                Statement* _statement;

                BlockId _bb1;
                BlockId _bb2;

                /* struct Branch { */
                /*     BlockId true_block; */
                /*     BlockId false_block; */
                /* }; */

                /* struct Load { */
                /*     Reg dst; */
                /* }; */

                /* union { */
                /*     Branch branch; */
                /*     Load load; */
                /* }; */

                // Offset for jump instructions or constant table indices for loads
                int _value;

                std::vector<kore::Reg> _registers;
        };

        std::ostream& operator<<(std::ostream& os, const Instruction instruction);
    }
}

#endif // KORE_KIR_INSTRUCTION_HPP
