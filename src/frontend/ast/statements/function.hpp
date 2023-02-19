#ifndef KORE_FUNCTION_HPP
#define KORE_FUNCTION_HPP

#include <vector>

#include "ast/expressions/parameter.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/statement_list.hpp"
#include "pointer_types.hpp"

namespace kore {
    class Identifier;
    class Type;

    class Function : public Statement {
        public:
            using body_iterator = StatementList::iterator;

        public:
            Function();
            Function(bool exported);
            Function(bool exported, const Token& token);
            virtual ~Function();

            std::string name() const;
            const Identifier* identifier() const;
            bool exported() const noexcept;
            int arity() const;
            FunctionType* type() const;
            const Type* return_type() const;

            const Identifier* parameter(int param_index);
            void add_parameter(Parameter* parameter);
            void set_return_type(Type* type);
            void add_statement(Owned<Statement> statement) override;
            Statement* last_statement();

            body_iterator begin();
            body_iterator end();

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            Identifier _name;
            bool _exported;
            Owned<FunctionType> _type;
            StatementList _body;
            std::vector<Statement*> _returns;
    };
}

#endif // KORE_FUNCTION_HPP
