#include "ast/ast_visitor.hpp"
#include "ast/statements/return_statement.hpp"

namespace kore {
    Return::Return() {}

    Return::Return(ExpressionList exprs)
        : Statement(StatementType::Return),
          _exprs(std::move(exprs))
    {
        for (const auto& expr : exprs) {
            location().merge(expr->location());
        }
    }

    Return::~Return() {}

    int Return::expr_count() const noexcept {
        return _exprs.size();
    }

    Expression* Return::get_expr(int idx) {
        return _exprs[idx].get();
    }

    const Expression* Return::get_expr(int idx) const {
        return _exprs[idx].get();
    }

    Return::iterator Return::begin() {
        return _exprs.begin();
    }

    Return::iterator Return::end() {
        return _exprs.begin();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(Return)
}
