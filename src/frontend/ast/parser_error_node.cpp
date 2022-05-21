#include "ast/ast_visitor.hpp"
#include "ast/parser_error_node.hpp"
#include "token_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    ParserErrorNode::ParserErrorNode(const std::string& msg, Location location)
        : Expression(ExpressionType::Error, location),
        _message(msg) {
    }

    ParserErrorNode::~ParserErrorNode() {}

    bool ParserErrorNode::is_error() const noexcept {
        return true;
    }

    std::string ParserErrorNode::message() const {
        return _message;
    }

    void ParserErrorNode::accept(AstVisitor& visitor) {
        UNUSED_PARAM(visitor);
    }

    void ParserErrorNode::accept_visit_only(AstVisitor& visitor) {
        UNUSED_PARAM(visitor);
    }
}
