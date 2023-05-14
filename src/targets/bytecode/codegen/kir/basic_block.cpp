#include "targets/bytecode/codegen/kir/basic_block.hpp"

namespace kore {
    namespace kir {
        BasicBlock::BasicBlock(BlockId id) : id(id) {}

        BlockId BasicBlock::StartBlockId = 0;
        BlockId BasicBlock::EndBlockId = 1;
        BlockId BasicBlock::InvalidBlockId = -1;
    }
}
