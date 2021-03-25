#include "ast_node.hpp"

AstNode::AstNode() : _location(Location::unknown) {}

AstNode::AstNode(const Location& location) : _location(location) {}

AstNode::~AstNode() {}

Location AstNode::location() const {
    return _location;
}
