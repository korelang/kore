#ifndef KORE_AST_VISITOR_HPP
#define KORE_AST_VISITOR_HPP

#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"

class AstVisitor {
    public:
        AstVisitor();
        virtual ~AstVisitor();

        // Expressions
        virtual void visit(ArrayExpression* expr);
        virtual void visit(ArrayFillExpression* expr);
        virtual void visit(ArrayRangeExpression* expr);
        virtual void visit(BinaryExpression* expr);
        virtual void visit(BoolExpression* expr);
        virtual void visit(CharExpression* expr);
        virtual void visit(FloatExpression* expr);
        virtual void visit(Identifier* expr);
        virtual void visit(IntegerExpression* expr);
        virtual void visit(StringExpression* expr);
        virtual void visit(UnaryExpression* expr);

        virtual bool precondition(ArrayExpression* expr);
        virtual bool precondition(ArrayFillExpression* expr);
        virtual bool precondition(ArrayRangeExpression* expr);
        virtual bool precondition(BinaryExpression* expr);
        virtual bool precondition(BoolExpression* expr);
        virtual bool precondition(CharExpression* expr);
        virtual bool precondition(FloatExpression* expr);
        virtual bool precondition(Identifier* expr);
        virtual bool precondition(IntegerExpression* expr);
        virtual bool precondition(StringExpression* expr);
        virtual bool precondition(UnaryExpression* expr);

        // Statements
        virtual void visit(Branch* statement);
        virtual void visit(Function* statement);
        virtual void visit(IfStatement* statement);
        virtual void visit(ImportStatement* statement);
        virtual void visit(ModuleStatement* statement);
        //virtual void visit(ReturnStatement* statement);
        virtual void visit(VariableAssignment* statement);
        virtual void visit(VariableDeclaration* statement);

        virtual bool precondition(Branch* statement);
        virtual bool precondition(Function* statement);
        virtual bool precondition(IfStatement* statement);
        virtual bool precondition(ImportStatement* statement);
        virtual bool precondition(ModuleStatement* statement);
        //virtual void precondition(ReturnStatement* statement);
        virtual bool precondition(VariableAssignment* statement);
        virtual bool precondition(VariableDeclaration* statement);
};

#endif // KORE_AST_VISITOR_HPP
