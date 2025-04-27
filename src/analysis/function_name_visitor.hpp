#ifndef KORE_FUNCTION_NAME_VISITOR_HPP
#define KORE_FUNCTION_NAME_VISITOR_HPP

#include "ast/ast.hpp"
#include "ast/ast_visitor.hpp"

namespace kore {
    namespace analysis {
        struct FunctionAnalysisResult {
            int func_index;
            const Function* func;
        };

        using FunctionMap = std::map<std::string, FunctionAnalysisResult>;

        /// Visit and collect all functions in an AST. Used when compiling a
        /// source file where we need to know all function names apriori before
        /// compiling each function so that the order of functions is
        /// irrelevant, i.e. a called function does not have to be defined
        /// before the caller funnction
        /// TODO: This will not work for more than one module. We need to
        /// provide a starting index or keep track of a global index in the
        /// class to get consecutive function indcies
        class FunctionNameVisitor final : public AstVisitor {
            public:
                FunctionMap collect_functions(const Ast& ast);

            private:
                FunctionMap _functions;

                void visit(Function& func) override;
        };
    }
}

#endif // FUNCTION_NAME_VISITOR_HPP
