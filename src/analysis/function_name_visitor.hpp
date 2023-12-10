#ifndef KORE_FUNCTION_NAME_VISITOR_HPP
#define KORE_FUNCTION_NAME_VISITOR_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"

#include <utility>

namespace kore {
    namespace analysis {
        using FunctionMap = std::map<std::string, std::pair<int, const Function*>>;

        /// Visit and collect all functions in an AST. Used when compiling a
        /// source file where we need to know all function names apriori before
        /// compiling each function so that the order of functions is
        /// irrelevant, i.e. a called function does not have to be defined
        /// before the caller funnction
        class FunctionNameVisitor final : public AstVisitor {
            public:
                FunctionMap collect_functions(const Ast& ast);

            private:
                FunctionMap _functions;

                void visit(Function& statement) override;
        };
    }
}

#endif // FUNCTION_NAME_VISITOR_HPP
