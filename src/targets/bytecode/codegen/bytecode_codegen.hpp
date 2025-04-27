#ifndef KORE_BYTECODE_GENERATOR_HPP
#define KORE_BYTECODE_GENERATOR_HPP

#include <memory>
#include <set>
#include <vector>

#include "ast/expressions/string_expression.hpp"
#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"
#include "targets/bytecode/codegen/bytecode.hpp"
#include "targets/bytecode/codegen/bytecode_array_writer.hpp"
#include "targets/bytecode/compiled_object.hpp"
#include "targets/bytecode/module.hpp"
#include "targets/bytecode/register.hpp"
#include "types/scope.hpp"

namespace kore {
    class Module;

    class BytecodeGenerator final : public AstVisitor {
        public:
            using reg_iterator = std::vector<Reg>::iterator;

        public:
            BytecodeGenerator();
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
            void visit(StringExpression& expr) override;
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
            std::vector<Reg> _destination_register_stack;
            BytecodeArrayWriter _writer;
            ScopeStack _scope_stack;

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

            // The set of globals (indices) used in this module. Used in the
            // VM to allocate space for a global table of values
            std::set<int> _global_indices;

        private:
            void reset();
            void push_register(Reg reg);
            void push_destination_register(Reg reg);
            Reg get_register_operand();
            Reg get_destination_register();
            Reg get_destination_register_or_allocate();
            reg_iterator get_register_operands(int count);
            Bytecode get_binop_instruction(
                TypeCategory type_category,
                BinOp binop
            );
            CompiledObject* current_object();
            void start_function_compile(const Function& func);
            void end_function_compile();
            void add_global_index(Reg reg);

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
