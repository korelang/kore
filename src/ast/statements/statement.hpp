#ifndef KORE_STATEMENT_HPP
#define KORE_STATEMENT_HPP

#include "ast/ast_node.hpp"
#include "ast/scanner/token.hpp"
#include "pointer_types.hpp"

namespace kore {
    class Expression;
    class Identifier;
    class Function;
    class Return;
    class Type;

    enum class StatementType {
        ModuleDecl,
        ImportDecl,
        VariableDecl,
        VariableAssignment,
        Function,
        Return,
    };

    /// Base class for all statements
    class Statement : public AstNode {
        public:
            using pointer = Owned<Statement>;

        public:
            Statement();
            Statement(StatementType statement_type);
            Statement(SourceLocation location, StatementType statement_type);
            virtual ~Statement();

            virtual void add_statement(Owned<Statement> statement);

            StatementType statement_type() const;

            template<typename T, typename... Args>
            static Owned<T> make_statement(Args... args) {
                return std::make_unique<T>(std::forward<Args>(args)...);
            }

        private:
            StatementType _statement_type;
    };
}

#endif // KORE_STATEMENT_HPP
