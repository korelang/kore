#include "ast/ast_writer.hpp"
#include "ast/parser_error_node.hpp"
#include "token_type.hpp"

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

    void ParserErrorNode::write(AstWriter* const writer) {
        writer->write("<Error>");
    }
}
