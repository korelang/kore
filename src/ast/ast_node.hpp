#ifndef KORE_AST_NODE_HPP
#define KORE_AST_NODE_HPP

#include "source_location.hpp"

namespace kore {
    class AstWriter;
    class AstVisitor;

    /// Base class for all nodes in an abstract syntax tree
    class AstNode {
        public:
            AstNode();
            AstNode(const SourceLocation& location);
            virtual ~AstNode();

            SourceLocation location() const;

            virtual void accept(AstVisitor& visitor);
            virtual void accept_visit_only(AstVisitor& visitor);

        protected:
            SourceLocation _location;
    };
}

#endif // KORE_AST_NODE_HPP
