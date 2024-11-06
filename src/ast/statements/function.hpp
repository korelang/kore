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
            FunctionType* type();
            const FunctionType* type() const;

            const Parameter* parameter(int idx) const;
            void add_parameter(Owned<Parameter>&& parameter);
            /* void set_return_type(const Type* type); */
            void add_statement(Owned<Statement> statement) override;
            Statement* last_statement();

            body_iterator begin();
            body_iterator end();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            Identifier _name;
            bool _exported;
            FunctionType* _type;
            std::vector<Owned<Parameter>> _parameters;
            StatementList _body;
            std::vector<Statement*> _returns;
    };
}

#endif // KORE_FUNCTION_HPP
