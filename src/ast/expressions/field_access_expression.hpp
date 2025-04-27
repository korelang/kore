#ifndef KORE_FIELD_ACCESS_EXPRESSION_HPP
#define KORE_FIELD_ACCESS_EXPRESSION_HPP

#include "ast/expressions/expression.hpp"

namespace kore {
  // A field access (a.b, a.["c" + "d"] etc.)
  class FieldAccessExpression : public Expression {
      public:
          FieldAccessExpression(Owned<Expression> obj, Owned<Expression> field_expr);
          virtual ~FieldAccessExpression();

          Expression* obj();
          Expression* field_expr();
          const Type* type() const override;

          KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

      private:
          // The expression being accessed
          Owned<Expression> _obj;

          // The field expression
          Owned<Expression> _field_expr;
  };
}

#endif // KORE_FIELD_ACCESS_EXPRESSION_HPP
