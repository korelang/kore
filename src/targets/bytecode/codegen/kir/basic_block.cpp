#include "targets/bytecode/codegen/kir/basic_block.hpp"

namespace kore {
    namespace kir {
        BasicBlock::BasicBlock(BlockId id) : id(id) {}

        bool BasicBlock::is_start_or_end_block() const {
            return id == StartBlockId || id == EndBlockId;
        }

        BlockId BasicBlock::StartBlockId = 0;
        BlockId BasicBlock::EndBlockId = 1;
        BlockId BasicBlock::InvalidBlockId = -1;
    }
}
