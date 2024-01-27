#include "function_name_visitor.hpp"

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
            _functions[statement.name()] = std::make_pair(_functions.size() + 1, &statement);
        }
    }
}
