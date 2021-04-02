#ifndef KORE_IMPORT_STATEMENT_HPP
#define KORE_IMPORT_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "token.hpp"

class AstWriter;
class Identifier;

class ImportStatement : public Statement {
    public:
        ImportStatement(Identifier* import_spec);
        virtual ~ImportStatement();

        /// The imported spec e.g. 'system.env'
        std::string spec() const;

        void write(AstWriter* const writer) override;

    private:
        std::unique_ptr<Identifier> _spec;
};

#endif // KORE_IMPORT_STATEMENT_HPP
