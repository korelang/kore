#ifndef KORE_BACKWARDS_DATAFLOW_ANALYSIS_HPP
#define KORE_BACKWARDS_DATAFLOW_ANALYSIS_HPP

#include "analysis/dataflow_analysis.hpp"
#include "utils/undirected_graph.hpp"

namespace kore {
    template<typename VertexData, typename EdgeData = void, template<typename> class GraphType = UndirectedGraph>
    class BackwardsDataflowAnalysis : public DataflowAnalysis<VertexData, GraphType> {
        public:
          void init(const UndirectedGraph<VertexData, EdgeData>& cfg) override;
          void solve() override;
    };
}

#endif // KORE_BACKWARDS_DATAFLOW_ANALYSIS_HPP
