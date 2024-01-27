#ifndef KORE_IMPORT_STATEMENT_HPP
#define KORE_IMPORT_STATEMENT_HPP

#include "pointer_types.hpp"
#include "ast/statements/statement.hpp"
#include "ast/scanner/token.hpp"

namespace kore {
    class AstWriter;
    class Identifier;

    class ImportStatement : public Statement {
        public:
            ImportStatement(Owned<Identifier> import_spec);
            virtual ~ImportStatement();

            /// The imported spec e.g. 'system.env'
            std::string spec() const;

            Identifier* identifier();

            KORE_AST_VISITOR_ACCEPT_METHOD_DEFAULT_DEFINITION

        private:
            Owned<Identifier> _spec;
    };
}

#endif // KORE_IMPORT_STATEMENT_HPP
