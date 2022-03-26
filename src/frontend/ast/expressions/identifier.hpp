#ifndef KORE_IDENTIFIER_HPP
#define KORE_IDENTIFIER_HPP

#include <vector>

#include "expression.hpp"

namespace kore {
    class Identifier : public Expression {
        public:
            Identifier(const Token& token);
            Identifier(const std::string& value);
            Identifier(const std::string& value, const Location& location);
            Identifier(const std::vector<std::string>& value, const Location& location);
            virtual ~Identifier();

            std::string name() const;
            std::string qualified_name() const;
            bool is_qualified() const noexcept;
            std::size_t size() const noexcept;
            const Type* type() const override;

            void accept(AstVisitor* visitor) override;
            void write(AstWriter* const writer) override;

        protected:
            // Used by function parameters that get constructed with a
            // type and a different ExpressionType
            Identifier(const Token& token, Type* type);

        private:
            std::string _value;
            std::vector<std::string> _parts;
    };
}

#endif // KORE_IDENTIFIER_HPP
