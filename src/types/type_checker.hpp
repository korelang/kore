#ifndef TYPE_CHECKER_HPP
#define TYPE_CHECKER_HPP

#include <vector>

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"

/// Contains all the information about a type error
struct TypeError {
    TypeError(const std::string& message, const Location& location);

    std::string message;

    Location location;

    // Used for e.g. multiple declaration errors to point to the previous
    // declaration
    Location other_location;
};

class VariableDeclaration;
class VariableAssignment;
class BinaryExpression;

class TypeChecker final : public AstVisitor {
    public:
        TypeChecker();
        virtual ~TypeChecker();

        /// Typecheck an AST
        int check(const Ast& ast);

        std::vector<TypeError> errors();

    private:
        // How many errors to tolerate before bailing out
        int _error_threshold = -1;

        std::vector<TypeError> _errors;

        void push_error(const std::string& message, const Location& location);
        void push_error(
            const std::string& message,
            const Location& location,
            const Location& otherLocation
        );

        void visit(BinaryExpression* expr) override;

        /* void visit(IfStatement* statement) override; */
        void visit(VariableAssignment* statement) override;

        bool check_statement(Statement* const statement);
        bool check_variable_declaration(VariableDeclaration* const decl);
        bool check_variable_assignment(VariableAssignment* const assign);

        bool check_expression(const Expression* const expression);
        bool check_binary_expression(const BinaryExpression* const binexpr);
};

#endif // TYPE_CHECKER_HPP