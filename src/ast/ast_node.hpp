#ifndef KORE_AST_NODE_HPP
#define KORE_AST_NODE_HPP

#include "source_location.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    /// In what context is a value being visited? E.g. an identifier can appear
    /// on both the left- and right-hand side of an assignment
    enum class ValueContext {
        LValue = 0,
        RValue,
    };

    // TODO: Do we need the old accept method anymore?

    #define KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION\
        void accept(AstVisitor& visitor) override;\
        void accept(AstVisitor& visitor, ValueContext context) override;

    #define KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(_class)\
        void _class::accept(AstVisitor& visitor) {\
            visitor.visit(*this);\
        }\
        void _class::accept(AstVisitor& visitor, ValueContext context) {\
            UNUSED_PARAM(context);\
            visitor.visit(*this);\
        }

    #define KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_STATEMENT_IMPL(_class)\
        void _class::accept(AstVisitor& visitor) {\
            visitor.visit(*this);\
        }

    #define KORE_AST_VISITOR_ACCEPT_METHOD_VALUE_CONTEXT_DEFINITION\
        void accept(AstVisitor& visitor, ValueContext context) override;

    #define KORE_AST_VISITOR_ACCEPT_METHOD_VALUE_CONTEXT_IMPL(_class)\
        void _class::accept(AstVisitor& visitor, ValueContext context) {\
            visitor.visit(*this, context);\
        }

    class AstWriter;
    class AstVisitor;

    /// Base class for all nodes in an abstract syntax tree
    class AstNode {
        public:
            AstNode();
            AstNode(const SourceLocation& location);
            virtual ~AstNode();

            SourceLocation& location();
            const SourceLocation& location() const;
            void set_location(const SourceLocation& location);

            /// A name for the ast node such as "my_func" or "+"
            virtual std::string name() const;

            /// A string representation of the ast node as given in the source
            /// such as "1 + 2"
            // TODO: Could either get the string via the scanner lines and
            // source location or create it from the expression. What about
            // locations spanning multiple lines?
            virtual std::string to_string() const;

            virtual void accept(AstVisitor& visitor);
            virtual void accept(AstVisitor& visitor, ValueContext context);

        protected:
            SourceLocation _location = SourceLocation::unknown;
    };
}

#endif // KORE_AST_NODE_HPP
