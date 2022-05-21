#ifndef KORE_AST_NODE_HPP
#define KORE_AST_NODE_HPP

#include "location.hpp"

namespace kore {
    class AstWriter;
    class AstVisitor;

    /// Base class for all nodes in an abstract syntax tree
    class AstNode {
        public:
            AstNode();
            AstNode(const Location& location);
            virtual ~AstNode();

            Location location() const;

            virtual void accept(AstVisitor& visitor);
            virtual void accept_visit_only(AstVisitor& visitor);

        protected:
            Location _location;
    };
}

#endif // KORE_AST_NODE_HPP
