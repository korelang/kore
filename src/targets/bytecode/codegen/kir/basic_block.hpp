#ifndef KORE_KIR_BASIC_BLOCK_HPP
#define KORE_KIR_BASIC_BLOCK_HPP

#include "targets/bytecode/codegen/kir/block_id.hpp"
#include "targets/bytecode/codegen/kir/instruction.hpp"

#include <vector>

namespace kore {
    namespace kir {
        using BlockId = int;

        /// A basic block in the KIR
        struct BasicBlock {
            BasicBlock(BlockId id);

            BlockId id;
            std::vector<Instruction> instructions;

            bool is_start_or_end_block() const;

            static BlockId StartBlockId;
            static BlockId EndBlockId;
            static BlockId InvalidBlockId;
        };

    }
}

#endif // KORE_KIR_BASIC_BLOCK_HPP
