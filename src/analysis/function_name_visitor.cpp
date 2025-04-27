#include "function_name_visitor.hpp"
#include "ast/statements/function.hpp"

namespace kore {
    namespace analysis {
        FunctionMap FunctionNameVisitor::collect_functions(const Ast& ast) {
            _functions.clear();

            for (auto const& statement : ast) {
                statement->accept(*this);
            }

            return _functions;
        }

        void FunctionNameVisitor::visit(Function& func) {
            int func_index = _functions.size() + 1;

            _functions[func.name()] = { func_index, &func };
        }
    }
}
