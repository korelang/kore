#ifndef KORE_UNDIRECTED_GRAPH_HPP
#define KORE_UNDIRECTED_GRAPH_HPP

#include "utils/directed_graph.hpp"
    
namespace kore {
    template<typename VertexData, typename EdgeData = void>
    class UndirectedGraph : public DirectedGraph<VertexData, EdgeData> {
        public:
            UndirectedGraph();
            virtual ~UndirectedGraph();

        protected:
            std::vector<VertexId> _predecessors;
    };
}

#endif // KORE_UNDIRECTED_GRAPH_HPP
