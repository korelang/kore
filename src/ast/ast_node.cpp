#include "ast_node.hpp"
#include "ast_visitor.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    AstNode::AstNode() : _location(SourceLocation::unknown) {}

    AstNode::AstNode(const SourceLocation& location) : _location(location) {}

    AstNode::~AstNode() {}

    SourceLocation& AstNode::location() {
        return _location;
    }

    const SourceLocation& AstNode::location() const {
        return _location;
    }

    void AstNode::set_location(const SourceLocation& location) {
        _location = location;
    }

    void AstNode::accept(AstVisitor& visitor) {
        UNUSED_PARAM(visitor);
    }

    void AstNode::accept(AstVisitor& visitor, ValueContext context) {
        UNUSED_PARAM(visitor);
        UNUSED_PARAM(context);
    }
}
