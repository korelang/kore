#ifndef KORE_STATEMENT_HPP
#define KORE_STATEMENT_HPP

#include "ast/ast_node.hpp"
#include "token.hpp"

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
            using pointer = std::unique_ptr<Statement>;

        public:
            Statement();
            Statement(Location location, StatementType statement_type);
            virtual ~Statement();

            virtual void add_statement(Statement* statement);

            StatementType statement_type() const;

            static Function* make_function(bool exported, const Token&);
            static Return* make_return(Expression* expr);
            static Statement* make_module_decl(const std::string& module_name, const Location& location);
            static Statement* make_module_decl(const Token& token);
            static Statement* make_import_decl(Identifier*);
            static Statement* make_variable_decl(const Token& identifier, const Token& type);
            static Statement* make_variable_assignment(const Token& identifier, Type* type, Expression* expr);
            static Statement* make_function_call(Expression* expression);

        private:
            StatementType _statement_type;
    };
}

#endif // KORE_STATEMENT_HPP
