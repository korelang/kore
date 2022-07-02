#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <map>
#include <memory>
#include <vector>

#include "frontend/ast/ast.hpp"
#include "frontend/ast/ast_visitor.hpp"
#include "codegen/bytecode/bytecode.hpp"
#include "codegen/bytecode/bytecode_array_writer.hpp"
#include "codegen/compiled_object.hpp"
#include "module.hpp"
#include "register.hpp"
#include "types/scope.hpp"

namespace kore {
    class Module;

    class BytecodeGenerator final : public AstVisitor {
        public:
            using reg_iterator = std::vector<const Reg>::iterator;

        public:
            BytecodeGenerator(ScopeStack& scope_stack);
            virtual ~BytecodeGenerator();

            // Compile a single statement e.g. for the REPL
            CompiledObject::pointer&& compile(Statement& statement);

            // Compile an AST into a module
            Module::pointer&& compile(const Ast& ast);

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
            void visit(Function& func) override;
            void visit(Return& ret) override;

        private:
            static const std::string _bytecode_version;
            static const std::string _MAIN_FUNC_NAME;

            std::vector<Reg> _register_stack;
            BytecodeArrayWriter _writer;
            ScopeStack& _scope_stack;

            // Current object we are compiling code for
            CompiledObject* _current_object;

            // TODO: Move this into Module class as well
            std::vector<CompiledObject::pointer> _objects;

            // Function names are stored in a vector based on the order they
            // are encountered so we can return to the previous function after
            // generating code for one. This may be another function and not
            // necessarily <main> since we want to support nested functions.
            std::vector<std::string> _functions;

            // Current module being compiled
            Module::pointer _module;

        private:
            void reset();
            void push_register(Reg reg);
            Reg get_register_operand();
            reg_iterator get_register_operands(int count);
            Bytecode get_binop_instruction(
                TypeCategory type_category,
                BinOp binop
            );
            CompiledObject* current_object();
            void start_function_compile(const Function& func);
            void end_function_compile();

            template<typename ...Args>
            void new_compiled_object(const std::vector<CompiledObject::pointer>& type, Args&&... args) {
                type.push_back(std::make_unique<CompiledObject>(std::forward<Args>(args)...));

                // If there is only one compiled object set it as the
                // current one
                if (type.size() == 1) {
                    _current_object = type.front().get();
                }
            }

            void new_function_from_name(const std::string& name);
    };
}

#endif // KORE_BYTECODE_GENERATOR_HPP
