#ifndef KORE_LINEAR_SCAN_REGISTER_ALLOCATOR_HPP
#define KORE_LINEAR_SCAN_REGISTER_ALLOCATOR_HPP

#include "analysis/live_variable_analysis.hpp"

namespace kore {
    namespace kir {
        struct asc_endpoint_compare {
            bool operator()(const LiveRange& live_range1, const LiveRange& live_range2);
        };

        /// An adaptation of the algorithm from "Linear Scan Register Allocation"
        /// by Massimiliano Poletto and Vivek Sarkar with the following
        /// differences: 
        ///
        /// 1. We are mapping a set of virtual registers, initially naively
        ///    allocated as needed, to a set of virtual registers that are a subset
        ///    of the initial set of virtual registers.
        /// 2. We don't handle spilling since we are always working with virtual registers.
        ///
        /// Otherwise the algorithm is implemented as in the paper. We don't waste
        /// time with expensive graph colouring algorithms as the performance gain
        /// is a guaranteed zero: all virtual registers are equally fast. This
        /// algorithm was chosen because it was easy to implement, should do a
        /// decent job of allocation (functions using less registers is still good
        /// in terms of avoiding running out of registers), and for my own
        /// educational purposes.
        class LinearScanRegisterAllocator final {
            public: 
                LinearScanRegisterAllocator();

                // Given a set of live ranges and a cfg, allocate registers
                void allocate(
                    const std::vector<LiveRange>& live_ranges,
                    const ControlFlowGraph& cfg
                );

            private:
                // A stack of free registers that are available for allocation
                std::stack<Reg> _free_registers;

                // A sorted list of the currently used registers, sorted by each live range's endpoint
                std::set<LiveRange, asc_endpoint_compare> _active_live_ranges;

                // A mapping from a live range to its allocated register
                std::unordered_map<LiveRange, Reg> _register_mapping;

                void expire_old_live_ranges(const LiveRange& live_range);

                void update_registers(
                    const std::vector<LiveRange> live_ranges,
                    const ControlFlowGraph& cfg
                );

                void replace_register(Reg reg1, Reg reg2); 
        };
    }
}

#endif // KORE_LINEAR_SCAN_REGISTER_ALLOCATOR_HPP
