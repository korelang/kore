#ifndef KORE_STATEMENT_HPP
#define KORE_STATEMENT_HPP

#include "ast/ast_node.hpp"
#include "token.hpp"

class Expression;
class Function;

/// Base class for all statements
class Statement : public AstNode {
    public:
        Statement();
        Statement(Location location);
        virtual ~Statement();

        static Function* make_function(bool exported, const Token&);
        static Statement* make_module_decl(const std::string& module_name, const Location& location);
        static Statement* make_import_decl(const std::string& import_spec, const Location& location);
        static Statement* make_variable_decl(const Token& identifier, const Token& type);
        static Statement* make_variable_assignment(const Token& identifier, const Token& type, Expression* expr);
};

#endif // KORE_STATEMENT_HPP
