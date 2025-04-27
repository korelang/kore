#ifndef KORE_AST_VISITOR_HPP
#define KORE_AST_VISITOR_HPP

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
    class Branch;
    class Function;
    class Call;
    class IfStatement;
    class ImportStatement;
    class ModuleStatement;
    class Return;
    class VariableAssignment;
    class VariableDeclaration;
    class ExpressionStatement;

    // TODO: Split this up into AstExprVisitor and AstStatementVisitor classes
    class AstVisitor {
        public:
            AstVisitor();
            virtual ~AstVisitor();

            // Expressions
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

            // Statements
            virtual void visit(Branch& statement);
            virtual void visit(Function& statement);
            virtual void visit(class Call& statement);
            virtual void visit(IfStatement& statement);
            virtual void visit(ImportStatement& statement);
            virtual void visit(ModuleStatement& statement);
            virtual void visit(Return& statement);
            virtual void visit(VariableAssignment& statement);
            virtual void visit(VariableDeclaration& statement);
            virtual void visit(ExpressionStatement& exprstmt);
    };
}

#endif // KORE_AST_VISITOR_HPP
