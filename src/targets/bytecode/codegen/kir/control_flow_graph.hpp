#ifndef KORE_CONTROL_FLOW_GRAPH_HPP
#define KORE_CONTROL_FLOW_GRAPH_HPP

#include "utils/undirected_graph.hpp"
#include "targets/bytecode/codegen/kir/basic_block.hpp"

namespace kore {
    namespace kir {
        /// A control-flow graph of kore intermediate representation (kir) basic blocks
        class ControlFlowGraph : public UndirectedGraph<BasicBlock> {
            public:
                using iterator = std::vector<BlockId>::iterator;
                using const_iterator = std::vector<BlockId>::const_iterator;

            public:
                ControlFlowGraph();
                virtual ~ControlFlowGraph();

                void add_edge(BlockId src, BlockId dst);
                BlockId add_block();
                BlockId add_block_as_current();
                BlockId add_block(BlockId id);
                BlockId add_block(BasicBlock& bb);
                bool has_block(BlockId id);
                BasicBlock& operator[](BlockId id);
                BasicBlock& current_block();
                void set_current_block(BlockId id);
                std::size_t size() const;

                bool has_predecessors(BlockId id) const;
                iterator predecessor_begin(BlockId id);
                iterator predecessor_end(BlockId id);
                bool has_successors(BlockId id) const;
                iterator successor_begin(BlockId id);
                iterator successor_end(BlockId id);
                const_iterator successor_cbegin(BlockId id) const;
                const_iterator successor_cend(BlockId id) const;

                void write_adjacency_lists(std::ostream& os) const;
                void write_graphviz(std::ostream& os);

            private:
                BlockId _current_block;
                std::vector<BasicBlock> _blocks;
        };
    }
}

#endif // KORE_CONTROL_FLOW_GRAPH_HPP
