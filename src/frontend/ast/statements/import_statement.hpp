#ifndef KORE_IMPORT_STATEMENT_HPP
#define KORE_IMPORT_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "token.hpp"
#include "pointer_types.hpp"

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

            void accept(AstVisitor& visitor) override;
            void accept_visit_only(AstVisitor& visitor) override;

        private:
            Owned<Identifier> _spec;
    };
}

#endif // KORE_IMPORT_STATEMENT_HPP
