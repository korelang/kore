#ifndef KORE_DATAFLOW_ANALYSIS_HPP
#define KORE_DATAFLOW_ANALYSIS_HPP

#include <vector>

#include "utils/graph.hpp"

namespace kore {
    /// The base class for all dataflow analyses
    template<typename VertexData, template<typename> class GraphType>
    class DataflowAnalysis {
        static_assert(std::is_base_of_v<Graph, GraphType>, "GraphType must be a class derived from Graph");
        
        public:
          virtual void init(const GraphType<VertexData>& cfg);
          virtual void solve();
    };

    /* auto analyzer = LiveVariableAnalysis(); */
    /* analyzer.init(cfg); */
    /* analyzer.solve(); */
    
    /* /// Dataflow analyses operating on the intermediate representation */
    /* class IRDataflowAnalysis { */
    /*     public: */
    /*       void init(); */
    /*       void solve(); */
    /* }; */
}

#endif // KORE_DATAFLOW_ANALYSIS_HPP
