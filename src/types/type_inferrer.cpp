#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    TypeInferrer::TypeInferrer() {}

    TypeInferrer::~TypeInferrer() {}

    void TypeInferrer::infer(Ast& ast) {
        for (auto& statement : ast) {
            statement->accept(this);
        }
    }

    bool TypeInferrer::precondition(VariableAssignment* statement) {
        // Do not infer types for a variable assignment with an explicit type
        return statement->type()->category() != TypeCategory::Unknown;
    }

    /* void TypeInferrer::visit(ArrayExpression* expr) {} */
    /* void TypeInferrer::visit(ArrayFillExpression* expr) {} */
    /* void TypeInferrer::visit(ArrayRangeExpression* expr) {} */

    void TypeInferrer::visit(BinaryExpression* expr) {
        expr->set_type(expr->left()->type()->unify(expr->right()->type()));
    }

    // Nothing to infer for literals, they already know their types
    /* void TypeInferrer::visit(BoolExpression* expr) {} */
    /* void TypeInferrer::visit(CharExpression* expr) {} */
    /* void TypeInferrer::visit(StringExpression* expr) {} */
    /* void TypeInferrer::visit(FloatExpression* expr) {} */
    /* void TypeInferrer::visit(IntegerExpression* expr) {} */

    /* void TypeInferrer::visit(Identifier* expr) {} */

    void TypeInferrer::visit(UnaryExpression* expr) {
        expr->set_type(expr->expr()->type());
    }

    /* void TypeInferrer::visit(Branch* statement) {} */
    /* void TypeInferrer::visit(Function* statement) {} */
    /* void TypeInferrer::visit(IfStatement* statement) {} */
    /* void TypeInferrer::visit(ImportStatement* statement) {} */
    /* void TypeInferrer::visit(ModuleStatement* statement) {} */
    //void TypeInferrer::visit(ReturnStatement* statement) {}

    void TypeInferrer::visit(VariableAssignment* statement) {
        statement->set_type(statement->expression()->type());
    }

    /* void TypeInferrer::visit(VariableDeclaration* statement) {} */
}
