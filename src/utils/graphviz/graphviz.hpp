#ifndef KORE_GRAPHVIZ_HPP
#define KORE_GRAPHVIZ_HPP

#include <ostream>

#include "targets/bytecode/codegen/kir/graph.hpp"

namespace kore {
    namespace dot {
        void write_graph(const kir::Graph& graph, std::ostream& os);
    }
}

#endif // KORE_GRAPHVIZ_HPP
