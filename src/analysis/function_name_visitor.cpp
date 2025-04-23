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
            int func_index = _functions.size() + 1;

            _functions[statement.name()] = { func_index, &statement };
        }
    }
}
