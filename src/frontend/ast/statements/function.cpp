#include "ast/ast_writer.hpp"
#include "ast/statements/function.hpp"

Function::Function()
    : _name("<missing>", Location::unknown),
      _exported(false),
      _return_type(nullptr) {
}

Function::Function(bool exported, const Token& token)
    : _name(token),
      _exported(exported),
      _return_type(nullptr) {

}

Function::Function(bool exported)
    : _name("<missing>", Location::unknown),
      _exported(exported),
      _return_type(nullptr) {
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

/* StatementList Function::body() { */
/*     return _body; */
/* } */

void Function::add_parameter(Expression* parameter) {
    _parameters.emplace_back(dynamic_cast<Identifier*>(parameter));
}

void Function::add_statement(Statement* statement) {
    _body.emplace_back(std::move(statement));
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

    writer->write(") {");

    // Write function body
    writer->newline();
    writer->write("    ");

    for (const auto& statement : _body) {
        statement->write(writer);
    }
    
    writer->dedent();
    writer->write("}");
}
