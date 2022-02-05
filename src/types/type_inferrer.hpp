#ifndef TYPE_INFERRER_HPP
#define TYPE_INFERRER_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"

namespace kore {
    /// Class that tries to infer the types of nodes in an AST
    class TypeInferrer final : public AstVisitor {
        public:
            TypeInferrer();
            virtual ~TypeInferrer();

            void infer(Ast& ast);

        private:
            /* void visit(ArrayExpression* expr) override; */
            /* void visit(ArrayFillExpression* expr) override; */
            /* void visit(ArrayRangeExpression* expr) override; */
            void visit(BinaryExpression* expr) override;
            /* void visit(BoolExpression* expr) override; */
            /* void visit(CharExpression* expr) override; */
            /* void visit(FloatExpression* expr) override; */
            /* void visit(Identifier* expr) override; */
            /* void visit(IntegerExpression* expr) override; */
            /* void visit(StringExpression* expr) override; */
            void visit(UnaryExpression* expr) override;

            bool precondition(VariableAssignment* statement) override;

            /* void visit(Branch* statement) override; */
            /* void visit(Function* statement) override; */
            /* void visit(IfStatement* statement) override; */
            /* void visit(ImportStatement* statement) override; */
            /* void visit(ModuleStatement* statement) override; */
            //void visit(ReturnStatement* statement);
            void visit(VariableAssignment* statement) override;
            /* void visit(VariableDeclaration* statement) override; */
    };
}

#endif // TYPE_INFERRER_HPP
