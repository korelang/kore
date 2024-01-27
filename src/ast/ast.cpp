#include "ast.hpp"
#include "ast/ast_writer.hpp"

namespace kore {
    Ast::Ast() {}

    Ast::Ast(Ast&& ast)
        : _module_name(ast._module_name),
        _module_path(ast._module_path),
        _statements(std::move(ast._statements)) {
    }

    Ast::Ast(const fs::path& path)
        : _module_path(path) {
    }

    Ast::~Ast() {
    }

    std::string Ast::module_name() const {
        return _module_name;
    }

    fs::path Ast::path() const {
        return _module_path;
    }

    bool Ast::is_main() const noexcept {
        return _module_name == "main";
    }

    void Ast::add_statement(Owned<Statement> statement) {
        _statements.emplace_back(std::move(statement));
    }

    Ast::ConstIter Ast::begin() const {
        return _statements.cbegin();
    }

    Ast::ConstIter Ast::end() const {
        return _statements.cend();
    }

    void Ast::write(AstWriter& writer) const {
        for (const auto& statement : _statements) {
            statement->accept(writer);
        }
    }
}
