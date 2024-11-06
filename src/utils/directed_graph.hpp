#ifndef KORE_DIRECTED_GRAPH_HPP
#define KORE_DIRECTED_GRAPH_HPP

#include "utils/graph.hpp"
    
namespace kore {
    template<typename VertexData, typename EdgeData = void>
    class DirectedGraph : public Graph<VertexData, EdgeData> {
        public:
            DirectedGraph();
            virtual ~DirectedGraph();
    };
}

#endif // KORE_DIRECTED_GRAPH_HPP
