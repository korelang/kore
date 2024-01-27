#ifndef KORE_PARSER_ERROR_NODE_HPP
#define KORE_PARSER_ERROR_NODE_HPP

#include "ast/expressions/expression.hpp"
#include "source_location.hpp"

namespace kore {
    /// AST node to denote syntax errors
    class ParserErrorNode final : public Expression {
        public:
            ParserErrorNode(const std::string& msg, SourceLocation location);
            virtual ~ParserErrorNode();

            bool is_error() const noexcept override;
            std::string message() const;

        private:
            std::string _message;
    };
}

#endif // KORE_PARSER_ERROR_NODE_HPP
