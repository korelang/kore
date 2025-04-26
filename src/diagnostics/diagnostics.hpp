#ifndef KORE_DIAGNOSTICS_HPP
#define KORE_DIAGNOSTICS_HPP

#include "ast/expressions/array_expression.hpp"
#include "ast/expressions/index_expression.hpp"
#include "ast/expressions/binary_expression.hpp"
#include "ast/statements/variable_assignment.hpp"

// TODO: Rename file to 'type_errors.hpp'
namespace kore {
    class Expression;

    struct IncompatibleTypes {
        const Expression* const expr1;
        const Expression* const expr2;
    };

    struct ArrayIndexNotNumeric {
        const IndexExpression* expr;
        const Expression* const index_expr;
    };

    struct ArrayIndexNeedsCast {
        const IndexExpression* expr;
        const Expression* const index_expr;
    };

    struct CannotAssign {
        const VariableAssignment* assignment;
        const int lhs_idx;
    };

    struct UnknownCall {
        const class Call* call;
    };

    /* struct ParamterArgumentCountMismatch { */
    /*     const class Call* call; */
    /*     const FunctionType* */ 
    /* }; */

    struct IncompatibleBinaryOperation {
        enum Context {
            Left,
            Right,
            Both
        };

        const Expression* const left;
        const Expression* const right;
        const BinOp op;
        const Context context;
    };

    struct ReturnTypeMismatch {
        const Function* function;
        const Return* ret;
        int idx;
    };

    struct VoidReturnFromNonVoidFunction {
        const Function* function;
        const Return* ret;
    };

    struct CannotDeclareMutableGlobal {
        const Identifier* identifier;
    };

    struct CannotAssignGlobalVariable {
        const Identifier* identifier;
        const Identifier* global_identifier;
    };

    struct VariableShadows {
        const Identifier* identifier;
        const Identifier* shadowed_identifier;
    };

    struct ConstantVariableRedeclaration {
        const Identifier* identifier;
        const Identifier* previous_identifier;
    };

    struct UndefinedVariable {
        const Identifier* identifier;
    };

    struct CannotCallNonFunction {
        const class Call* call;
        const Type* given_type;
    };

    struct IncorrectArgumentCount {
        const class Call* call;
        const FunctionType* func_type;
    };

    struct IncorrectArgumentType {
        const class Call* call;
        const Expression* arg;
        const Type* parameter_type;
        const int parameter_idx;
    };
}

#endif // KORE_DIAGNOSTICS_HPP
