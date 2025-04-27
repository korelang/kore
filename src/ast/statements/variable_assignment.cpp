#include "ast/ast_visitor.hpp"
#include "ast/statements/variable_assignment.hpp"
#include "ast/expressions/expression.hpp"
#include "types/function_type.hpp"
#include "types/type.hpp"

namespace kore {
    VariableAssignment::VariableAssignment(
        std::vector<Owned<Expression>> lhs_exprs,
        std::vector<Owned<Expression>> rhs_exprs
    ) : _lhs_exprs(std::move(lhs_exprs)), _rhs_exprs(std::move(rhs_exprs)) {
        location().merge(_lhs_exprs[0]->location());
        location().merge(_rhs_exprs.back()->location());
    }

    VariableAssignment::~VariableAssignment() {}

    int VariableAssignment::lhs_count() const {
        return _lhs_exprs.size();
    }

    int VariableAssignment::rhs_count() const {
        return _rhs_exprs.size();
    }

    Expression* VariableAssignment::lhs(int idx) {
        return _lhs_exprs[idx].get();
    }

    Expression* VariableAssignment::rhs(int idx) {
        return _rhs_exprs[idx].get();
    }

    const Expression* VariableAssignment::lhs(int idx) const {
        return _lhs_exprs[idx].get();
    }

    const Expression* VariableAssignment::rhs(int idx) const {
        return _rhs_exprs[idx].get();
    }

    const Type* VariableAssignment::rhs_type(int idx) const {
        int rhs_idx = 0;

        // Search for the corresponding type of a right-hand side expression. This handles
        // complex assignments such as:
        //
        // a, b, c, d, e = f(), g(), 1
        for (const auto& expr : _rhs_exprs) {
            // TODO: Handle other types of destructuring like arrays
            if (expr->expr_type() == ExpressionType::Call) {
                auto func_type = expr->type()->as<FunctionType>();

                if (idx < rhs_idx + func_type->return_arity()) {
                    return func_type->return_type(idx - rhs_idx);
                } else {
                    rhs_idx += func_type->return_arity();
                }
            } else {
                if (idx == rhs_idx) {
                    return expr->type();
                }

                ++rhs_idx;
            }
        }

        return Type::unknown();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(VariableAssignment)
}
