#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <stack>

#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/codegen.hpp"
#include "backend/vm/vm.hpp"

namespace kore {
    class BytecodeGenerator final : public Codegen<Bytecode> {
        public:
            BytecodeGenerator();
            virtual ~BytecodeGenerator();

            void emit_version() override;
            void emit_header() override;
            /* void write(Bytecode code) override; */
            void emit(Bytecode bytecode) override;

            void visit(BinaryExpression* expr) override;
            void visit(BoolExpression* expr) override;
            void visit(IntegerExpression* expr) override;
            void visit(FloatExpression* expr) override;
            void visit(VariableAssignment* statement) override;

        private:
            // For now, we just use a very simple register allocator with
            // infinite registers that just bumps a register count
            int _registers = 0;

            std::stack<Reg> _register_stack;

            const static std::string _bytecode_version;

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
