#include "ast_node.hpp"
#include "ast_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstNode::AstNode() : _location(Location::unknown) {}

    AstNode::AstNode(const Location& location) : _location(location) {}

    AstNode::~AstNode() {}

    Location AstNode::location() const {
        return _location;
    }

    void AstNode::accept(AstVisitor& visitor) {
        UNUSED_PARAM(visitor);
    }
}
