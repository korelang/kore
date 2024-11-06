#ifndef KORE_GRAPH_HPP
#define KORE_GRAPH_HPP

#include <vector>

namespace kore {
    using VertexId = unsigned int;
    
    template<typename VertexData>
    struct Vertex {
        VertexId _id;
        VertexData _data;
     };
    
    /// Base graph class for all other types of graphs
    template<typename VertexData, typename EdgeData = void>
    class Graph {
        using Vertex = Vertex<VertexData>;

        public:
            Graph();
            virtual ~Graph();
        
            void add_vertex(VertexData vt);
            void add_vertex(Vertex v);
            void remove_vertex(VertexData vt);
            void remove_vertex(Vertex v);

            void add_edge(Vertex source, Vertex destination);
            void add_edge(VertexId source, VertexId destination);
            void remove_edge(Vertex source, Vertex destination);
            void remove_edge(VertexId source, VertexId destination);

            void has_edge(Vertex source, Vertex destination);
            bool has_edge(VertexId source, VertexId destination);
        
        protected:
            // Might want to make the vector contain a pair of a vertex and some edge data?
            std::vector<std::pair<Vertex, EdgeData>> _successors;
    };

    /* template<typename VertexData> */
    /* struct bfs_iterator { */
    /*     using iterator_category = std::forward_iterator_tag; */
    /*     using difference_type   = std::ptrdiff_t; */
    /*     using value_type        = VertexData; */
    /*     using pointer           = VertexData*; */
    /*     using reference         = VertexData&; */
        
    /*     reference operator*() const { return *m_ptr; } */
    /*     pointer operator->() { return m_ptr; } */
    /*     Iterator& operator++() { m_ptr++; return *this; } */  
    /*     Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; } */
    /*     friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }; */
    /*     friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }; */
        
    /*     private: */
    /*         Graph* _graph; */
    /*         VertexData* _cur_vertex; */
            
    /*         void next_vertex() { */
    /*         } */
    /* }; */
}

/* struct Node { */
    
/* }; */

/* enum class EscapeState { */
/*     Argument, */
/*     ArgumentIndirect, */
/*     Return, */
/*     ReturnIndirect, */
/*     MaybeEscape */
/* }; */

/* enum class ConnectivityType { */
/*     Reference, */
/*     Deferred, */
/*     Field */
/* }; */

/* struct ConnectivityEdge { */
/*     ConnectivityType type; */
/* }; */

/* class ConnectionGraph : public DirectedGraph<Node, ConnectivityEdge> {}; */

#endif
