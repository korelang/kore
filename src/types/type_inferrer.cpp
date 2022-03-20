#include "type_inferrer.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "types/unknown_type.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    TypeInferrer::TypeInferrer(ScopeStack& scope_stack)
        : _scope_stack(scope_stack) {}

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

    void TypeInferrer::visit(BinaryExpression* expr) {
        expr->set_type(expr->left()->type()->unify(expr->right()->type()));
    }

    void TypeInferrer::visit(Identifier* expr) {
        auto entry = _scope_stack.find(expr->name());

        // If no identifier was found, this is an undefined variable which is
        // caught by the type checker
        if (entry) {
            expr->set_type(entry->identifier->type());
        } else {
            expr->set_type(Type::unknown());
        }
    }

    void TypeInferrer::visit(UnaryExpression* expr) {
        expr->set_type(expr->expr()->type());
    }

    void TypeInferrer::visit(VariableAssignment* statement) {
        auto expr_type = statement->expression()->type();

        // Set the type of the identifier/variable and save it in the symbol
        // table
        statement->set_type(expr_type);
        _scope_stack.insert(statement->identifier());
    }

    bool TypeInferrer::precondition(Branch* branch) {
        UNUSED_PARAM(branch);
        _scope_stack.enter(false);
        return false;
    }

    bool TypeInferrer::postcondition(Branch* branch) {
        UNUSED_PARAM(branch);
        _scope_stack.leave();
        return false;
    }
}
