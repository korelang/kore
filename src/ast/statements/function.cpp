#include <memory>

#include "ast/ast_writer.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/function.hpp"
#include "types/function_type.hpp"
#include "types/type.hpp"
#include "types/unknown_type.hpp"

namespace kore {
    Function::Function()
        : Statement(SourceLocation::unknown, StatementType::Function),
        _name("<missing>"),
        _exported(false),
        _type(nullptr) {
    }

    Function::Function(bool exported, const Token& token)
        : Statement(token.location(), StatementType::Function),
        _name(token),
        _exported(exported),
        _type(nullptr) {
    }

    Function::Function(bool exported)
        : Statement(SourceLocation::unknown, StatementType::Function),
        _name("<missing>"),
        _exported(exported),
        _type(nullptr) {
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

    const FunctionType* Function::type() const {
        return _type;
    }

    FunctionType* Function::type() {
        return _type;
    }

    const Parameter* Function::parameter(int idx) const {
        return _parameters[idx].get();
    }

    void Function::add_parameter(Owned<Parameter>&& parameter) {
        _parameters.emplace_back(std::move(parameter));
    }

    void Function::add_statement(Owned<Statement> statement) {
        // If the statement is a return, also save it separately. This
        // is useful for type-checking etc. later on
        if (statement->statement_type() == StatementType::Return) {
            _returns.push_back(statement.get());
        }

        _body.emplace_back(std::move(statement));
    }

    Statement* Function::last_statement() {
        return _body.size() > 0 ? _body.back().get() : nullptr;
    }

    Function::body_iterator Function::begin() {
        return _body.begin();
    }

    Function::body_iterator Function::end() {
        return _body.end();
    }

    KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_IMPL(Function)
}
