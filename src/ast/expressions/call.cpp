#include <sstream>

#include "ast/ast_visitor.hpp"
#include "ast/expressions/call.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Call::Call(Owned<Expression> identifier, std::vector<Owned<Expression>> parameters)
        : Expression(ExpressionType::Call, identifier->location()),
          _identifier(std::move(identifier))
    {
        for (auto& expr : parameters) {
            _parameters.emplace_back(std::move(expr));
        }
    }

    Call::~Call() {}

    std::string Call::name() const {
        return static_cast<Identifier*>(_identifier.get())->name();
    }

    Expression* Call::arg(int index) {
        return _parameters[index].get();
    }

    /* std::vector<Expression*> Call::parameters() { */
    /*     return _parameters; */
    /* } */

    int Call::arg_count() const {
        return _parameters.size();
    }

    std::string Call::expected_func_type_name() const {
        std::ostringstream oss;

        oss << "(";

        for (int i = 0; i < arg_count(); ++i) {
            auto& param = _parameters[i];

            oss << param->type()->name();

            if (i != arg_count() - 1) {
                oss << ", ";
            }
        }

        oss << ")";

        return oss.str();
    }

    const Type* Call::type() const {
        return _type;
    }

    void Call::accept(AstVisitor& visitor) {
        accept_visit_only(visitor);
    }

    void Call::accept_visit_only(AstVisitor& visitor) {
        visitor.visit(*this);
    }
}
