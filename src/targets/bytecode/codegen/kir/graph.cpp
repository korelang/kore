#include <ostream>

#include "targets/bytecode/codegen/kir/graph.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    namespace kir {
        Graph::Graph() {}
        Graph::~Graph() {}

        void Graph::add_edge(BlockId src, BlockId dst) {
            if (!has_block(src) || !has_block(dst)) {
                throw std::runtime_error("Cannot add edge to non-existent basic block");
            }

            _successors[src].push_back(dst);
            _predecessors[dst].push_back(src);
        }

        BlockId Graph::add_block() {
            auto bb = BasicBlock(_blocks.size());
            _blocks.push_back(bb);

            return bb.id;
        }

        BlockId Graph::add_block_as_current() {
            auto id = add_block();
            set_current_block(id);

            return id;
        }

        BlockId Graph::add_block(BlockId id) {
            auto bb = BasicBlock(id);
            _blocks.push_back(bb);

            return bb.id;
        }

        BlockId Graph::add_block(BasicBlock& bb) {
            bb.id = _blocks.size();
            _blocks.push_back(bb);

            return bb.id;
        }

        bool Graph::has_block(BlockId id) {
            // TODO: Perhaps use a u32 and use the max value for invalid blocks
            return id >= 0 && id < static_cast<BlockId>(_blocks.size());
        }

        BasicBlock& Graph::operator[](BlockId id) {
            return _blocks[id];
        }

        BasicBlock& Graph::current_block() {
            return _blocks[_current_block];
        }

        void Graph::set_current_block(BlockId id) {
            _current_block = id;
        }

        std::size_t Graph::size() const {
            return _blocks.size();
        }

        bool Graph::has_predecessors(BlockId id) const {
            return _predecessors.find(id) != _predecessors.cend();
        }

        Graph::iterator Graph::predecessor_begin(BlockId id) {
            return _predecessors[id].begin();
        }

        Graph::iterator Graph::predecessor_end(BlockId id) {
            return _predecessors[id].end();
        }

        bool Graph::has_successors(BlockId id) const {
            return _successors.find(id) != _successors.cend();
        }

        Graph::iterator Graph::successor_begin(BlockId id) {
            return _successors[id].begin();
        }

        Graph::iterator Graph::successor_end(BlockId id) {
            return _successors[id].end();
        }

        Graph::const_iterator Graph::successor_cbegin(BlockId id) const {
            return _successors.at(id).cbegin();
        }

        Graph::const_iterator Graph::successor_cend(BlockId id) const {
            return _successors.at(id).cend();
        }

        void Graph::write_adjacency_lists(std::ostream& os) const {
            // Write the instructions in each basic block
            for (auto& bb : _blocks) {
                if (bb.id == BasicBlock::StartBlockId) {// || bb.id == BasicBlock::EndBlockId) {
                    continue;
                }

                if (bb.instructions.size() > 0) {
                    os << "instructions for bb" << bb.id << std::endl;

                    for (auto& instruction : bb.instructions) {
                        os << instruction << std::endl;
                    }
                } else {
                    os << "no instructions in bb" << bb.id;
                }

                os << std::endl;
            }

            // Write the adjacency list
            for (auto& bb : _blocks) {
                os << "bb" << bb.id << ": ";

                if (has_successors(bb.id)) {
                    for (auto succ = successor_cbegin(bb.id); succ < successor_cend(bb.id); ++succ) {
                        os << "bb" << *succ << " ";
                    }
                } else {
                    os << "no successors";
                }

                os << std::endl;
            }
        }

        void Graph::write_graphviz(std::ostream& os) {
            UNUSED_PARAM(os);
        }
    }
}
