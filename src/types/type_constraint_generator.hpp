#ifndef KORE_TYPE_CONSTRAINT_GENERATOR_HPP
#define KORE_TYPE_CONSTRAINT_GENERATOR_HPP

#include <unordered_map>
#include <vector>

#include "ast/ast_node.hpp"
#include "types/type_constraint.hpp"
#include "types/type_variable.hpp"

namespace kore {
    class Ast;
    class Identifier;
    class ArrayExpression;
    class IndexExpression;
    class BinaryExpression;
    class Expression;
    class Call;
    class Return;
    class Function;
    class IfStatement;
    class VariableAssignment;

    using TypeConstraintSet = std::vector<TypeConstraint>;

    class TypeConstraintGenerator : public AstVisitor {
        public:
            TypeConstraintGenerator();
            virtual ~TypeConstraintGenerator();

            TypeConstraintSet get_constraints();

            void generate_constraints(Ast& ast);

            void visit(ArrayExpression& expr) override;
            void visit(IndexExpression& expr) override;
            void visit(IndexExpression& expr, ValueContext context) override;
            void visit(BinaryExpression& expr) override;
            void visit(Identifier& expr) override;
            void visit(Identifier& expr, ValueContext context) override;
            void visit(VariableAssignment& statement) override;
            void visit(IfStatement& ifstatement) override;
            void visit(kore::Function& func) override;
            void visit(class Call& call) override;
            void visit(Return& ret) override;

        private:
            TypeConstraintSet _constraints;
            std::unordered_map<TypeVariable, Identifier> _identifier_typevar_mapping;

            void set_expr_type(Expression& expr, Type* type);

            void add_constraint(const TypeConstraint& constraint);
            void add_constraint(Expression* expr, const TypeConstraint& constraint);

            void add_constraint_for_identifier(
                const TypeConstraint& constraint,
                const Identifier& identifier
            );

            TypeVariable* get_typevar_from_identifier(const Identifier& identifier);

            Type* visit_expression(Expression* expr);
    };
}

#endif // KORE_TYPE_CONSTRAINT_GENERATOR_HPP
