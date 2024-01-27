#include "ast/ast_visitor.hpp"
#include "ast/scanner/token_type.hpp"
#include "ast/parser_error_node.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    ParserErrorNode::ParserErrorNode(const std::string& msg, SourceLocation location)
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
}
