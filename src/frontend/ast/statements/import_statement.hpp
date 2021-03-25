#ifndef KORE_IMPORT_STATEMENT_HPP
#define KORE_IMPORT_STATEMENT_HPP

#include "ast/statements/statement.hpp"
#include "token.hpp"

class AstWriter;

class ImportStatement : public Statement {
    public:
        ImportStatement(const Token& token);
        ImportStatement(const std::string& import_spec, const Location& location);
        virtual ~ImportStatement();

        /// The imported spec e.g. 'system.env'
        std::string spec() const;

        void write(AstWriter* const writer) override;

    private:
        std::string _spec;
};

#endif // KORE_IMPORT_STATEMENT_HPP
