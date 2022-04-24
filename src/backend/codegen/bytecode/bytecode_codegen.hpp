#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <memory>
#include <vector>

#include "codegen/bytecode/bytecode.hpp"
#include "codegen/bytecode/bytecode_array_writer.hpp"
#include "codegen/codegen.hpp"
#include "codegen/compiled_object.hpp"
#include "register.hpp"
#include "types/scope.hpp"

namespace kore {
    class BytecodeGenerator final : public AstVisitor {
        public:
            using CompiledObjectPtr = std::unique_ptr<CompiledObject>;
            using RegIterator = std::vector<Reg>::iterator;

        public:
            BytecodeGenerator(ScopeStack& scope_stack);
            virtual ~BytecodeGenerator();

            void compile(const Ast& ast);

            template<typename OutputIter>
            void acquire_compiled_code(OutputIter it) {
                std::move(_objects.begin(), _objects.end(), it);
            }

            void visit(BinaryExpression& expr) override;
            void visit(BoolExpression& expr) override;
            void visit(IntegerExpression& expr) override;
            void visit(FloatExpression& expr) override;
            void visit(Identifier& identifier) override;
            void visit(VariableAssignment& assignment) override;
            void visit(IfStatement& ifstatement) override;
            void visit(class Call& call) override;
            void visit(Return& ret) override;

            bool precondition(Branch& statement) override;
            bool precondition(Function& statement) override;

            bool postcondition(Function& statement) override;

        private:
            const static std::string _bytecode_version;
            std::vector<Reg> _register_stack;
            BytecodeArrayWriter _writer;
            ScopeStack& _scope_stack;

            CompiledObject* _current_object;
            std::vector<CompiledObjectPtr> _objects;

        private:
            Reg get_register_operand();
            RegIterator get_register_operands(int count);
            void free_registers(int count);
            Bytecode get_binop_instruction(
                TypeCategory type_category,
                BinOp binop
            );
            CompiledObject* current_object();
            void start_function_compile(Function* func);
            void end_function_compile();

            template<typename ...Args>
            void new_compiled_object(Args&&... args) {
                _objects.push_back(std::make_unique<CompiledObject>(std::forward<Args>(args)...));
                
                // If there is only one compiled object set it as the
                // current one
                if (_objects.size() == 1) {
                    _current_object = _objects.front().get();
                }
            }
    };
}

#endif // KORE_BYTECODE_GENERATOR_HPP
