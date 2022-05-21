#ifndef KORE_MODULE_STATEMENT_HPP
#define KORE_MODULE_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "token.hpp"

namespace kore {
    class ModuleStatement : public Statement {
        public:
            ModuleStatement(const Token& token);
            ModuleStatement(const std::string& module_name, const Location& location);
            virtual ~ModuleStatement();

            std::string name() const;

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            std::string _name;
    };
}

#endif // KORE_MODULE_STATEMENT_HPP
