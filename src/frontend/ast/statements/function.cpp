#include "ast/ast_writer.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/function.hpp"
#include "types/type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    Function::Function()
        : Statement(Location::unknown, StatementType::Function),
        _name("<missing>"),
        _exported(false),
        _return_type(new UnknownType()) {
    }

    Function::Function(bool exported, const Token& token)
        : Statement(token.location(), StatementType::Function),
        _name(token),
        _exported(exported),
        _return_type(new UnknownType()) {

    }

    Function::Function(bool exported)
        : Statement(Location::unknown, StatementType::Function),
        _name("<missing>"),
        _exported(exported),
        _return_type(new UnknownType()) {
    }

    Function::~Function() {}

    std::string Function::name() const {
        return _name.name();
    }

    bool Function::exported() const noexcept {
        return _exported;
    }

    ParameterList Function::parameters() {
        return _parameters;
    }

    Type* Function::return_type() {
        return _return_type;
    }

    void Function::add_parameter(Parameter* parameter) {
        _parameters.push_back(parameter);
    }

    void Function::set_return_type(Type* type) {
        _return_type = type;
    }

    void Function::add_statement(Statement* statement) {
        _body.emplace_back(std::move(statement));
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

        for (const auto& parameter : parameters()) {
            parameter->write(writer);
            writer->write(", ");
        }

        writer->write(")");
        return_type()->write(writer);
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

    void Function::accept(AstVisitor* visitor) {
        if (!visitor->precondition(this)) {
            for (auto& statement : _body) {
                statement->accept(visitor);
            }
        }

        visitor->visit(this);

        visitor->postcondition(this);
    }
}
