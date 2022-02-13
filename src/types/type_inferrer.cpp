#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    TypeInferrer::TypeInferrer(SymbolTable& symbol_table)
        : _symbol_table(symbol_table) {}

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

    void TypeInferrer::visit(Identifier* expr) {
        auto identifier = _symbol_table.find_identifier(expr->name());

        // If no identifier was found, this is an undefined variable which is
        // caught by the type checker
        if (identifier) {
            expr->set_type(identifier->type());
        } else {
            expr->set_type(Type::unknown());
        }
    }

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
        auto expr_type = statement->expression()->type();

        // Set the type of the identifier/variable and save it in the symbol
        // table
        statement->set_type(expr_type);
        _symbol_table.insert_identifier(statement->identifier());
    }

    /* void TypeInferrer::visit(VariableDeclaration* statement) {} */
}
