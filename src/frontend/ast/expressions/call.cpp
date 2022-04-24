#include <sstream>

#include "ast/ast_visitor.hpp"
#include "ast/ast_writer.hpp"
#include "ast/expressions/call.hpp"
#include "utils/unused_parameter.hpp"

namespace kore {
    Call::Call(Identifier* identifier, std::vector<Expression*>& parameters)
        : Expression(ExpressionType::Call, identifier->location()),
          _name(identifier->name())
    {
        for (Expression* expr : parameters) {
            _parameters.emplace_back(expr);
        }
    }

    Call::~Call() {}

    std::string Call::name() const {
        return _name;
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

    void Call::write(AstWriter* const writer) {
        writer->write(name());
    }

    void Call::accept(AstVisitor* visitor) {
        visitor->visit(this);
    }
}
