#include <memory>

#include "ast/ast_writer.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/function.hpp"
#include "types/function_type.hpp"
#include "types/type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    Function::Function()
        : Statement(Location::unknown, StatementType::Function),
        _name("<missing>"),
        _exported(false),
        _type(std::make_unique<FunctionType>()) {
    }

    Function::Function(bool exported, const Token& token)
        : Statement(token.location(), StatementType::Function),
        _name(token),
        _exported(exported),
        _type(std::make_unique<FunctionType>()) {
    }

    Function::Function(bool exported)
        : Statement(Location::unknown, StatementType::Function),
        _name("<missing>"),
        _exported(exported),
        _type(std::make_unique<FunctionType>()) {
    }

    Function::~Function() {}

    std::string Function::name() const {
        return _name.name();
    }

    const Identifier* Function::identifier() const {
        return &_name;
    }

    bool Function::exported() const noexcept {
        return _exported;
    }

    int Function::arity() const {
        return _type->arity();
    }

    FunctionType* Function::type() const {
        return _type.get();
    }

    const Type* Function::return_type() {
        return _type->return_type();
    }

    const Identifier* Function::parameter(int param_index) {
        return _type->parameter(param_index);
    }

    void Function::add_parameter(Parameter* parameter) {
        _type->add_parameter(parameter);
    }

    void Function::set_return_type(Type* type) {
        _type->set_return_type(type);

        // After parsing the return type, as therefore the parameters,
        // set the type of the identifier associated with the function
        // which will later be saved in lexical scopes
        _name.set_type(_type.get());
    }

    void Function::add_statement(Statement* statement) {
        _body.emplace_back(std::move(statement));

        // If the statement is a return, also save it separately. This
        // is useful for type-checking etc. later on
        if (statement->statement_type() == StatementType::Return) {
            _returns.emplace_back(std::move(statement));
        }
    }

    Function::body_iterator Function::begin() {
        return _body.begin();
    }

    Function::body_iterator Function::end() {
        return _body.end();
    }

    void Function::write(AstWriter* const writer) {
        if (exported()) {
            writer->write("exported ");
        }

        writer->write("func ");
        writer->write(name());
        writer->write("(");

        for (int i = 0; i < arity(); ++i) {
            /* auto parameter = _type->parameter(i); */

            /* parameter->write(writer); */
            writer->write(", ");
        }

        writer->write(")");
        /* _type->return_type()->write(writer); */
        writer->write("{");

        // Write function body
        writer->newline();
        writer->write("    ");

        for (const auto& statement : _body) {
            statement->write(writer);
        }

        writer->dedent();
        writer->write("}");
        writer->newline();
    }

    void Function::accept(AstVisitor& visitor) {
        if (!visitor.precondition(*this)) {
            for (auto& statement : _body) {
                statement->accept(visitor);
            }
        }

        visitor.visit(*this);
        visitor.postcondition(*this);
    }
}
