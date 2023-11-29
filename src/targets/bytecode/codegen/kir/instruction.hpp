#ifndef KORE_KIR_INSTRUCTION_HPP
#define KORE_KIR_INSTRUCTION_HPP

#include "ast/expressions/expressions.hpp"
#include "ast/source_location.hpp"
#include "ast/statements/statement.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/kir/block_id.hpp"
#include "targets/bytecode/register.hpp"
#include "types/type_category.hpp"

#include <variant>

namespace kore {
    namespace kir {
        struct OneRegister {
            Reg reg;
        };

        struct TwoRegisters {
            Reg reg1;
            Reg reg2;
        };

        struct ThreeRegisters {
            Reg reg1;
            Reg reg2;
            Reg reg3;
        };

        struct RegisterAndValue {
            Reg reg;
            int value;
        };

        struct Value {
            int value;
        };

        struct CallV {
            Expression* expr;
            std::vector<kore::Reg> arg_registers;
            std::vector<kore::Reg> ret_registers;
        };

        struct ReturnV {
            std::vector<kore::Reg> registers;
        };

        using InstructionType = std::variant<
            OneRegister,
            TwoRegisters,
            ThreeRegisters,
            RegisterAndValue,
            Value,
            CallV,
            ReturnV
        >;

        /// A KIR instruction
        ///
        /// Whereas instructions emitted by code generation are lower-level
        /// executable instructions, KIR instructions exist at a higher level
        /// and are not executable but are used for optimisation and final code
        /// generation.
        ///
        /// The instruction types are a high-level representation of actual
        /// emitted code and different opcodes fit into these types, e.g. many
        /// opcodes for loading constants fit into the instruction type containing
        /// two registers.
        struct Instruction {
            Bytecode opcode;

            InstructionType type;
        };

        std::ostream& operator<<(std::ostream& os, const Instruction instruction);
    }
}

#endif // KORE_KIR_INSTRUCTION_HPP
