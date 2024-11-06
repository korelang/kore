#ifndef KORE_LIVE_VARIABLE_ANALYSIS_HPP
#define KORE_LIVE_VARIABLE_ANALYSIS_HPP

#include <queue>

#include "analysis/bitset.hpp"
#include "analysis/backwards_dataflow_analysis.hpp"
#include "analysis/control_flow_graph.hpp"
#include "targets/bytecode/codegen/kir/basic_block.hpp"
#include "targets/bytecode/codegen/kir/control_flow_graph.hpp"

namespace kore {
    struct LivenessState {
        bitset live_in;
        bitset live_out;
        bitset def;
        bitset use;
    };

    struct LivenessRange {
        Reg reg;
        unsigned int start;
        unsigned int end;
        
        bool is_dead() const {
            return start == end;
        }
    };

    class LiveVariableAnalysis : public BackwardsDataflowAnalysis<kir::BasicBlock, void, ControlFlowGraph> {
        public:
          void init(const ControlFlowGraph& cfg) override;
          void solve() override;

        private:
          // A map from each instruction to its current liveness state
          std::unordered_map<kir::Instruction, LivenessState> _liveness_states;
          std::queue<kir::Instruction> _worklist;
    };
}

#endif // KORE_LIVE_VARIABLE_ANALYSIS_HPP
