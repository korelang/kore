#ifndef KORE_AST_EXPR_VISITOR_HPP
#define KORE_AST_EXPR_VISITOR_HPP

namespace kore {
    enum class ValueContext;

    class Expression;
    class ArrayExpression;
    class ArrayFillExpression;
    class IndexExpression;
    class IndexExpression;
    class ArrayRangeExpression;
    class BinaryExpression;
    class BoolExpression;
    class CharExpression;
    class FloatExpression;
    class FieldAccessExpression;
    class Identifier;
    class Parameter;
    class IntegerExpression;
    class StringExpression;
    class UnaryExpression;

    class AstExprVisitor {
        public:
            AstExprVisitor();
            virtual ~AstExprVisitor();

            virtual void visit(Expression& expr, ValueContext context);
            virtual void visit(ArrayExpression& expr);
            virtual void visit(ArrayFillExpression& expr);
            virtual void visit(IndexExpression& expr);
            virtual void visit(IndexExpression& expr, ValueContext context);
            virtual void visit(ArrayRangeExpression& expr);
            virtual void visit(BinaryExpression& expr);
            virtual void visit(BoolExpression& expr);
            virtual void visit(CharExpression& expr);
            virtual void visit(FloatExpression& expr);
            virtual void visit(FieldAccessExpression& expr);
            virtual void visit(Identifier& expr);
            virtual void visit(Identifier& expr, ValueContext context);
            virtual void visit(Parameter& expr);
            virtual void visit(IntegerExpression& expr);
            virtual void visit(StringExpression& expr);
            virtual void visit(UnaryExpression& expr);
    };
}

#endif // KORE_AST_EXPR_VISITOR_HPP
