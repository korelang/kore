#ifndef KORE_FUNCTION_HPP
#define KORE_FUNCTION_HPP

#include <vector>

#include "ast/expressions/parameter.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"

namespace kore {
    class Identifier;
    class Type;

    using ParameterList = std::vector<Parameter*>;

    class Function : public Statement {
        public:
            using body_iterator = StatementList::iterator;

        public:
            Function();
            Function(bool exported);
            Function(bool exported, const Token& token);
            virtual ~Function();

            std::string name() const;
            bool exported() const noexcept;
            ParameterList parameters();
            Type* return_type();

            void add_parameter(Parameter* parameter);
            void set_return_type(Type* type);
            void add_statement(Statement* statment) override;

            body_iterator begin();
            body_iterator end();

            void write(AstWriter* const writer) override;
            void accept(AstVisitor* visitor) override;

        private:
            Identifier _name;
            bool _exported;
            ParameterList _parameters;
            Type* _return_type;
            StatementList _body;
    };
}

#endif // KORE_FUNCTION_HPP
