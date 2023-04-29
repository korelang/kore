#include "targets/bytecode/codegen/kir/basic_block.hpp"

namespace kore {
    namespace kir {
        BasicBlock::BasicBlock(BlockId id) : id(id) {}

        BlockId BasicBlock::InvalidBlockId = -1;
    }
}
