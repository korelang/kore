#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <stack>

#include "backend/codegen/bytecode/bytecode.hpp"
#include "backend/codegen/codegen.hpp"

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
            void visit(VariableAssignment* statement) override;

        private:
            // For now, we just use a very simple register allocator with
            // infinite registers that just bumps a register count
            int _registers = 0;

            int allocate_register();
            int get_register_operand();

            std::stack<int> _register_stack;

            const static std::string _bytecode_version;
    };
}

#endif // KORE_BYTECODE_GENERATOR_HPP
