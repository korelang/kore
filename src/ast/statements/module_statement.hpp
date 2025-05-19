#ifndef KORE_MODULE_STATEMENT_HPP
#define KORE_MODULE_STATEMENT_HPP

#include "ast/scanner/token.hpp"
#include "ast/statements/statement.hpp"

namespace kore {
    class ModuleStatement : public Statement {
        public:
            ModuleStatement(const Token& token);
            ModuleStatement(const std::string& module_name, const SourceLocation& location);
            virtual ~ModuleStatement();

            std::string name() const override;

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            std::string _name;
    };
}

#endif // KORE_MODULE_STATEMENT_HPP
