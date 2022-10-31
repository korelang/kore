#include "ast_node.hpp"
#include "ast_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstNode::AstNode() : _location(SourceLocation::unknown) {}

    AstNode::AstNode(const SourceLocation& location) : _location(location) {}

    AstNode::~AstNode() {}

    SourceLocation AstNode::location() const {
        return _location;
    }

    void AstNode::accept(AstVisitor& visitor) {
        UNUSED_PARAM(visitor);
    }

    void AstNode::accept_visit_only(AstVisitor& visitor) {
        UNUSED_PARAM(visitor);
    }
}
