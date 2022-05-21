#ifndef KORE_AST_VISITOR_HPP
#define KORE_AST_VISITOR_HPP

#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"

namespace kore {
    class AstVisitor {
        public:
            AstVisitor();
            virtual ~AstVisitor();

            // Expressions
            virtual void visit(ArrayExpression& expr);
            virtual void visit(ArrayFillExpression& expr);
            virtual void visit(ArrayRangeExpression& expr);
            virtual void visit(BinaryExpression& expr);
            virtual void visit(BoolExpression& expr);
            virtual void visit(CharExpression& expr);
            virtual void visit(FloatExpression& expr);
            virtual void visit(Identifier& expr);
            virtual void visit(Parameter& expr);
            virtual void visit(IntegerExpression& expr);
            virtual void visit(StringExpression& expr);
            virtual void visit(UnaryExpression& expr);

            virtual void visit_only(ArrayExpression& expr);
            virtual void visit_only(ArrayFillExpression& expr);
            virtual void visit_only(ArrayRangeExpression& expr);
            virtual void visit_only(BinaryExpression& expr);
            virtual void visit_only(BoolExpression& expr);
            virtual void visit_only(CharExpression& expr);
            virtual void visit_only(FloatExpression& expr);
            virtual void visit_only(Identifier& expr);
            virtual void visit_only(Parameter& expr);
            virtual void visit_only(IntegerExpression& expr);
            virtual void visit_only(StringExpression& expr);
            virtual void visit_only(UnaryExpression& expr);

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

            virtual void visit_only(Branch& statement);
            virtual void visit_only(Function& statement);
            virtual void visit_only(class Call& statement);
            virtual void visit_only(IfStatement& statement);
            virtual void visit_only(ImportStatement& statement);
            virtual void visit_only(ModuleStatement& statement);
            virtual void visit_only(Return& statement);
            virtual void visit_only(VariableAssignment& statement);
            virtual void visit_only(VariableDeclaration& statement);
    };
}

#endif // KORE_AST_VISITOR_HPP
