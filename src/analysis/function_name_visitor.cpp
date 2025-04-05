#include "function_name_visitor.hpp"
#include "ast/statements/function.hpp"
#include "targets/bytecode/vm/builtins/builtins.hpp"

namespace kore {
    namespace analysis {
        FunctionMap FunctionNameVisitor::collect_functions(const Ast& ast) {
            _functions.clear();

            for (auto const& statement : ast) {
                statement->accept(*this);
            }

            return _functions;
        }

        void FunctionNameVisitor::visit(Function& statement) {
            // Start user function indices after the number of builtin functions
            // TODO: Should we just have a separate instruction to load builtin
            // functions? Probably easier
            int func_index = vm::builtin_function_count() + _functions.size() + 1;

            _functions[statement.name()] = { func_index, &statement };
        }
    }
}
