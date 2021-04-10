#ifndef KORE_STATEMENT_HPP
#define KORE_STATEMENT_HPP

#include "ast/ast_node.hpp"
#include "token.hpp"

class Expression;
class Identifier;
class Function;
class Return;

/// Base class for all statements
class Statement : public AstNode {
    public:
        Statement();
        Statement(Location location);
        virtual ~Statement();

        static Function* make_function(bool exported, const Token&);
        static Return* make_return(Expression* expr);
        static Statement* make_module_decl(const std::string& module_name, const Location& location);
        static Statement* make_module_decl(const Token& token);
        static Statement* make_import_decl(Identifier*);
        static Statement* make_variable_decl(const Token& identifier, const Token& type);
        static Statement* make_variable_assignment(const Token& identifier, const Token& type, Expression* expr);
};

#endif // KORE_STATEMENT_HPP
