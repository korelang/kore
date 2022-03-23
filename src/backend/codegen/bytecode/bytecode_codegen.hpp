#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <stack>

#include "codegen/bytecode/bytecode.hpp"
#include "codegen/bytecode/bytecode_array_writer.hpp"
#include "codegen/codegen.hpp"
#include "register.hpp"
#include "types/scope.hpp"

namespace kore {
    class BytecodeGenerator final : public AstVisitor {
        public:
            BytecodeGenerator(ScopeStack& scope_stack);
            virtual ~BytecodeGenerator();

            void compile(const Ast& ast);

            void visit(BinaryExpression* expr) override;
            void visit(BoolExpression* expr) override;
            void visit(IntegerExpression* expr) override;
            void visit(FloatExpression* expr) override;
            void visit(Identifier* expr) override;
            void visit(VariableAssignment* statement) override;
            void visit(IfStatement* statement) override;

            bool precondition(Branch* statement) override;
            /* bool postcondition(Branch* statement) override; */

            code_iterator begin() const;
            code_iterator end() const;

        private:
            // For now, we just use a very simple register allocator with
            // infinite registers that just bumps a register count
            int _registers = 0;

            std::stack<Reg> _register_stack;

            const static std::string _bytecode_version;

            BytecodeArrayWriter _writer;

            ScopeStack& _scope_stack;

        private:
            Reg allocate_register();
            Reg get_register_operand();
            Bytecode get_binop_instruction(
                TypeCategory type_category,
                BinOp binop
            );
    };
}

#endif // KORE_BYTECODE_GENERATOR_HPP
