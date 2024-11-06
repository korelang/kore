#include "ast/ast.hpp"
#include "ast/expressions/expressions.hpp"
#include "ast/statements/statements.hpp"
#include "types/type_constraint_generator.hpp"

namespace kore {
    TypeConstraintGenerator::TypeConstraintGenerator() {}

    TypeConstraintGenerator::~TypeConstraintGenerator() {}

    TypeConstraintSet TypeConstraintGenerator::get_constraints() {
        return _constraints; 
    }

    void TypeConstraintGenerator::generate_constraints(Ast& ast) {
        for (auto& statement : ast) {
            statement->accept(*this);
        }
    }

    void TypeConstraintGenerator::visit(ArrayExpression& expr) {
    }

    void TypeConstraintGenerator::visit(IndexExpression& expr) {
    }

    void TypeConstraintGenerator::visit(IndexExpression& expr, ValueContext context) {
    }

    void TypeConstraintGenerator::visit(BinaryExpression& expr) {
        auto left_type = visit_expression(expr.left());
        auto right_type = visit_expression(expr.right());
        auto typevar = TypeVariable::fresh();

        set_expr_type(expr, typevar);

        if (left_type->is_type_variable() && right_type->is_type_variable()) {
            // If both operands are type variables, default to i32
            // TODO: Default depends on binary operator
            add_constraint(TypeConstraint { left_type, Type::i32() });
            add_constraint(TypeConstraint { right_type, Type::i32() });
        } else if (left_type->is_numeric()) {
            // If one of the operands is numeric, use it to dictate the inference
            add_constraint(TypeConstraint { right_type, left_type });
        } else if (right_type->is_numeric()) {
            add_constraint(TypeConstraint { left_type, right_type });
        } else {
            // Otherwise this is an unsatisfiable constraint which the type checker will catch
            add_constraint(TypeConstraint { left_type, right_type });
        }
    }

    void TypeConstraintGenerator::visit(Identifier& expr) {
        auto typevar = get_typevar_from_identifier(expr);

        if (!typevar) {
            // Undefined variable, handled in type checker
            // TODO: Early exit
        }

        /* return ???; // Do we use a stack again? */
    }

    void TypeConstraintGenerator::visit(Identifier& expr, ValueContext context) {
    }

    void TypeConstraintGenerator::visit(VariableAssignment& statement) {
        auto rhs_type = visit_expression(statement.rhs());
        auto typevar = TypeVariable::fresh();

        add_constraint_for_identifier(
            TypeConstraint { typevar, rhs_type },
            statement.lhs()
        );
    }

    void TypeConstraintGenerator::visit(IfStatement& ifstatement) {
        for (auto& branch : ifstatement) {
            auto condition = branch->condition();

            if (condition) {
                condition->accept(*this);
            }

            auto typevar = TypeVariable::fresh();

            add_constraint(TypeConstraint { typevar, Type::bool_type() });
            set_expr_type(condition, typevar);

            for (auto& statement : *branch) {
                statement->accept(*this);
            }
        }
    }

    void TypeConstraintGenerator::visit(kore::Function& func) {
        auto func_type = Type::make_function_type();

        // Create type variables for each function parameter and add them to the function type
        for (int i = 0; i < func.arity(); ++i) {
            auto parameter = func.parameter(i);
            auto arg_typevar = TypeVariable::fresh();

            func_type->add_parameter_type(arg_typevar);
            set_expr_type(parameter, arg_typevar);
        }

        // Create type variables for each function return type and add them to
        // the function type
        for (int i = 0; i < func.return_arity(); ++i) {
            auto return_type = func.return_type(i);
            auto ret_typevar = TypeVariable::fresh();

            func_type->add_return_type(ret_typevar);
            set_expr_type(parameter, ret_typevar);
        }

        // Visit the function body and generate type constraints
        for (auto& statement : func) {
            statement->accept(*this);
        }

        auto typevar = TypeVariable::fresh();

        add_constraint(TypeConstraint{ typevar, func_type });
        set_expr_type(func, typevar);
    }

    void TypeConstraintGenerator::visit(class Call& call) {
        auto typevar = TypeVariable::fresh();
        auto builtin_function = vm::get_builtin_function_by_name(call.name());

        // Check if this is a built-in function
        if (builtin_function) {
            // TODO: Make built-in functions use a FunctionType instead of
            // separate arg and ret types for compatibility with user-defined
            // functions
        } else {
            int func_index = _functions[call.name()].first;

            add_constraint(TypeConstraint{ typevar, func_type->type() });
            set_expr_type(call, typevar);
        }
    }

    void TypeConstraintGenerator::visit(Return& ret) {
        auto& func = current_function();

        // If the return statement returns an expression, generate code
        // for it, then get its register
        if (ret.expr()) {
            auto typevar = TypeVariable::fresh();

            add_constraint(TypeConstraint{ typevar, func->return_type() });
        } else {
            add_constraint(TypeConstraint{ typevar, Type::void_type() });
        }

        set_expr_type(ret, typevar);
    }

    void TypeConstraintGenerator::trace(const std::string& name, const std::string& msg) {
        if (_args && _args->trace == TraceOption::TypeConstraints) {
            std::string message = name;

            if (!msg.empty()) {
                message += " (" + msg + ")";
            }

            debug_group("type_constraints", "%s", message.c_str());
        }
    }

    Type* TypeConstraintGenerator::visit_expression(Expression* expr) {
        expr->accept(*this);

        return pop_register();
    }

    void TypeConstraintGenerator::add_constraint(const TypeConstraint& constraint) {
        _constraints.push_back(constraint);
    }

    void TypeConstraintGenerator::add_constraint(
        Expression* expr,
        const TypeConstraint& constraint
    ) {
        add_constraint(constraint);
        set_expr_type(*expr, constraint.type1);
    }

    void TypeConstraintGenerator::add_constraint_for_identifier(
        const TypeConstraint& constraint,
        const Identifier& identifier
    ) {
        add_constraint(constraint);

        _identifier_typevar_mapping[constraint. typevar1] = identifier;
    }

}
