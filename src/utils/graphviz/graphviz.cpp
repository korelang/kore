#include "utils/graphviz/graphviz.hpp"

namespace kore {
    namespace dot {
        void write_graph(const kir::Graph& graph, std::ostream& os) {
            os << "strict digraph {" << std::endl;
            os << "}";
        }
    }
}
