#include "ast/ast_writer.hpp"
#include "ast/ast_visitor.hpp"
#include "ast/statements/return_statement.hpp"

namespace kore {
    Return::Return() : _expr(nullptr) {}

    Return::Return(Expression* expr) : _expr(std::move(expr)) {}

    Return::~Return() {}

    Expression* Return::expr() {
        return _expr.get();
    }

    void Return::write(AstWriter* const writer) {
        writer->write("return<");
        _expr->write(writer);
        writer->write(">");
        writer->newline();
    }

    void Return::accept(AstVisitor& visitor) {
        if (_expr) {
            _expr->accept(visitor);
        }

        visitor.visit(*this);
    }
}
