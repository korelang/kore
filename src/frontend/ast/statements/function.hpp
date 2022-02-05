#ifndef KORE_FUNCTION_HPP
#define KORE_FUNCTION_HPP

#include <vector>

#include "ast/expressions/identifier.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"

namespace kore {
    class Identifier;
    class Type;

    using ParameterList = std::vector<Identifier*>;

    class Function : public Statement {
        public:
            Function();
            Function(bool exported);
            Function(bool exported, const Token& token);
            virtual ~Function();

            std::string name() const;
            bool exported() const noexcept;
            ParameterList parameters();
            Type* return_type();
            /* StatementList body(); */

            void add_parameter(Expression* parameter);
            void set_return_type(Type* type);
            void add_statement(Statement* statment) override;

            void write(AstWriter* const writer) override;

        private:
            Identifier _name;
            bool _exported;
            ParameterList _parameters;
            Type* _return_type;
            StatementList _body;
    };
}

#endif // KORE_FUNCTION_HPP
